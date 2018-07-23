#include <ctime>


    // julian day (days since January 1, 4713 ante Chr.)
    // 2440587.5 julian day for January 1, 1970 (unix epoch date)
    class Julian {
    public:
        // Default constructor: today's date as julian day
        Julian() {
            time_t seconds = time(0);
            date = 2440587.5 + seconds / 86400;
        }
        // valid only for gregorian calendar
        Julian(int year, unsigned month, unsigned day) {
            if (month < 3) {
                year -= 1;
                month += 12;
            }
            int a = year / 100;
            // calculate only for gregorian years
            int b = 2 - a + a / 4;
            date = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + b - 1524.5;
        }

        operator double() const {
            return this->date;
        }

        double operator-(const Julian &other) const {
            return this->date - other.date;
        }

        Julian operator-(long days) const {
            return Julian();
        }

    private:
        double date;
    };

// Then you can use the code more naturally:

int main() {
    Julian today, newYear(2014, 1, 1);
    std::cout << (today - newYear) << std::endl;
    return 0;
}
