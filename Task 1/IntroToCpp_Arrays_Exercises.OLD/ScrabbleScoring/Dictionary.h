#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace Aaron
{


    class Dictionary
    {
    public:

        Dictionary(const char* filename)
        {
            std::ifstream qfile(filename);
            std::set<std::string, InsensitiveCompare> words = { std::istream_iterator<std::string>(qfile), std::istream_iterator<std::string>() };
            m_words = words;
            qfile.close();
        };

        ~Dictionary()
        {

        }

        bool Contains(const char* word)
        {
            // std::cout << word << (m_words.find(word) != m_words.end() ? "TRUE" : "FALSE") << std::endl;
            return m_words.find(word) != m_words.end();
        }


    protected:
    private:

        struct InsensitiveCompare {
            bool operator() (const std::string& a, const std::string& b) const {
                return _stricmp(a.c_str(), b.c_str()) < 0;
            }
        };

        std::set<std::string, InsensitiveCompare> m_words;
    };
}



namespace Flynn
{

    class Dictionary
    {
    public:
        Dictionary(const char* filename)
        {
            std::string line;
            std::fstream file;
            file.open(filename);
            if (file.is_open())
            {
                while (std::getline(file, line)) {
                    dict.push_back(line);
                }
            }
            file.close();
        };

        ~Dictionary()
        {
        }

        std::string toUpper(std::string str) {

            std::string upperStr;
            for (auto& c : str)
            {
                upperStr.push_back(std::toupper(c));
            }
            return upperStr;
        }


        bool Contains(const char* word)
        {
            return std::binary_search(dict.begin(), dict.end(), toUpper(word));
        }

    private:

        std::vector<std::string> dict;
    };
}

namespace Daniel
{
    class Dictionary
    {
    public:
        Dictionary(const char* filename)
        {
            std::ifstream file;
            file.open(filename);
            if (file.is_open())
            {
                std::string word;
                while (file >> word)
                {
                    dictionary.push_back(word);
                    //std::cout << word << std::endl;
                }
            }
            file.close();
        };
        ~Dictionary()
        {
            dictionary.clear();
        }
        bool Contains(const char* word)
        {
            return binarySearch(0, dictionary.size() - 1, ToUpper(word)) >= 0;
        }

        int binarySearch(int left, int right, std::string word)
        {
            if (right >= left)
            {
                int middle = (left + right) / 2;
                if (dictionary[middle] == word)
                    return middle;
                if (dictionary[middle] > word)
                    return binarySearch(left, middle - 1, word);
                return binarySearch(middle + 1, right, word);
            }
            return -1;
        }

        std::string ToUpper(const std::string& s)
        {
            std::string lower;
            int size = s.size();
            for (int i = 0; i < size; i++)
                lower += toupper(s[i]);
            return lower;
        }

    protected:
        
        std::vector<std::string> dictionary;
    private:
    };
}