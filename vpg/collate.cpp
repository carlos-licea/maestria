#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str,line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream,cell,','))
            {
                m_data.push_back(cell);
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str,CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   
int main(int argc, char* argv[])
{
    std::string outfilename = argv[1];

    std::ifstream file("result");
    std::ofstream outfile(outfilename, std::ofstream::out);

    CSVRow row;
    while(file >> row)
    {
       float sum_dof = 0;
       float sum_duration = 0;
       for(int i = 0; i < 30; ++i) {
           sum_dof += stof(row[4]);
           sum_duration += stof(row[5]);
           file >> row;
       }
       auto q_fix = row[0];
       auto q_fluct = row[1];
       auto l = row[2];
       auto p = row[3];

       outfile << q_fix << "," << q_fluct << ","  << l << "," << p << ","<< sum_dof / 30.0f << "," << sum_duration << ",pg" << "\n" << std::flush;
       outfile << q_fix << "," << q_fluct << ","  << l << "," << p << ","<< row[4] << "," << row[5] << ",vpg" << "\n" << std::flush;
    }
}
