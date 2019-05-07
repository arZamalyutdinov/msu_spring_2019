#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <atomic>

class FileSorter {
    public:
    FileSorter(const std::string &inputFile, const std::string &outputFile, const uint64_t memorySize,
    const std::ios::openmode mode) : in(inputFile), out(outputFile), size(memorySize), numberOfFiles(0)
    {
        // std::cout << "i am here" << std::endl;
    }
    ~FileSorter() 
    {

    }

    void sort()
    {
        // std::cout << "i am here" << std::endl;
        std::mutex mtx;
        std::ifstream in;
        std::thread thr1(threadSort, std::ref(mtx), std::ref(in), size);
        std::thread thr2(threadSort, std::ref(mtx), std::ref(in), size);
        std::string s1, s2;
        {
            std::stringstream ss;
            ss << thr1.get_id() << '1';
            s1 = ss.str();
        }
        {
            std::stringstream ss;
            ss << thr2.get_id() << '1';
            s2 = ss.str();
        }
        thr1.join();
        thr2.join();

        auto fgood = [](std::string &str) -> bool {
            std::ifstream f(str, std::ios::binary);
            return f.good();
        };

        if (fgood(s1) && fgood(s2)) {
            merge(s1, s2, out);
            std::remove(s1.c_str());
            std::remove(s2.c_str());
        } else {
            if(fgood(s1)) {
                std::rename(s1.c_str(), out.c_str());
            } else {
                std::rename(s2.c_str(), out.c_str());
            }
        }
    }

    private:
    uint64_t size;
    std::string in;
    std::string out;
    std::atomic_int numberOfFiles;
    
    protected:
    static void threadSort(std::mutex &mtx, std::ifstream &in, uint64_t size)
    {
        std::thread::id thr_id = std::this_thread::get_id();
        std::stringstream ss;
        ss << thr_id;
        int fileNum = 0;
        while(in.good()) {
            std::unique_lock<std::mutex> lock(mtx);
            std::vector<uint64_t> vec;
        // std::cout << "i am here" << std::endl;
            for (uint64_t i = 0; i < size / 2; ++i) {
                uint64_t tmp;
                in.read((char*)(&tmp), sizeof(tmp));
                vec.push_back(tmp);
                if (!in.good()) {
                    break;
                }
            }
            if (vec.empty()) {
                break;
            }
            lock.unlock();
            std::sort(vec.begin(), vec.end());
            ss << thr_id << ++fileNum;
            std::ofstream ofile(ss.str(), std::ios::binary);
            ofile.write((char*)vec.data(), vec.size() * sizeof(*(vec.data())));
            std::this_thread::yield();
        }
        while (fileNum > 1) {
            std::unique_lock<std::mutex> lock(mtx);
            std::string str1 = ss.str() + std::to_string(fileNum--);
            std::string str2 = ss.str() + std::to_string(fileNum);
            std::string res = ss.str() + "res";
            merge(std::ref(str1),std::ref(str2),std::ref(res));
            lock.unlock();
            std::remove(str1.c_str());
            std::remove(str2.c_str());
            std::rename(res.c_str(), str2.c_str());
            std::this_thread::yield();
        }
    }

    static void merge(const std::string &str1, const std::string &str2, const std::string &res)
    {
        std::ifstream file1(str1, std::ios::binary);
        std::ifstream file2(str2, std::ios::binary);
        std::ofstream result(res, std::ios::binary);
        uint64_t f1, f2;
        if (file1.good() && file2.good()) {
            file1.read((char*) &f1, sizeof(f1));
            file2.read((char*) &f2, sizeof(f2));
        }
        for (; file1.good() && file2.good() ;) {
            if (f1 > f2) {
                result.write((char*)&f2, sizeof(f2));
                file2.read((char*) &f2, sizeof(f2));
            } else {
                result.write((char*)&f1, sizeof(f1));
                file1.read((char*) &f1, sizeof(f1));
            }
        }
        if (!file1.good()) {
            while(file2.good()) {
                result.write((char*)(&f2), sizeof(f2));
                file2.read((char*)(&f2), sizeof(f2));
            }
        } else {
            while(file1.good()) {
                result.write((char*)(&f1), sizeof(f1));
                file2.read((char*)(&f1), sizeof(f1));
            }
        }
        file1.close();
        file2.close();
        result.close();
    }
};

int main(int argc, char const **argv)
{
    std::cout << "Print size of memory: " << std::endl;
    uint64_t size;
    std::cin >> size;
    const std::string in = argv[1];
    const std::string out = argv[2];
    FileSorter srt(in, out, size, std::ios::binary);
    srt.sort();
    return 0;
}