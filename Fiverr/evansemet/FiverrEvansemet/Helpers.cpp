#include <cmath>
#include <chrono>
#include <string>
#include <iostream>
#include <array>


using namespace std;

const int monthDays[12]
= { 31, 28, 31, 30, 31, 30,
   31, 31, 30, 31, 30, 31 };

const int monthDaysLeapYear[12]
= { 31, 29, 31, 30, 31, 30,
   31, 31, 30, 31, 30, 31 };

const int monthDaysTotal[12] = { 31, 59, 90, 120, 151, 181, 212, 243,
273, 304, 334, 365 };

const int monthDaysTotalLeapYear[12] = { 31, 60, 91, 121, 152, 182, 213, 244,
274, 305, 335, 366 };

const array<string, 12> monthAbbrev = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP",
"OCT", "NOV", "DEC" };


class Helpers {
public:

    bool isLeapYear(int year) {
        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) return true;

        return false;
    }

    array<int, 3> getDateFromSerial(string serial) {

        long serialAsLong = stol(serial);

        long tempSerial = 0;
        int year = 1900;
        while (true) {
            if (isLeapYear(year) && year != 1900) {
                if ((tempSerial + 366) <= serialAsLong) {
                    tempSerial += 366;
                }
                else
                    break;

            }
            else {
                if ((tempSerial + 365) <= serialAsLong) {
                    tempSerial += 365;
                }
                else
                    break;
            }

            year++;
        }

        int days = serialAsLong - tempSerial;

        int month = 0;
        for (int i = 0; i < 12; i++) {
            if (isLeapYear(year)) {
                if (monthDaysTotalLeapYear[i] > days) {
                    month = i;
                    break;
                }
            }
            else {
                if (monthDaysTotal[i] > days) {
                    month = i;
                    break;
                }
            }
        }

        if (month != 0) {
            if (isLeapYear(year)) {
                /*if (month > 1) days++;*/
                days -= monthDaysTotalLeapYear[month - 1];
            }
            else days -= monthDaysTotal[month - 1];
        }


        array<int, 3> yearMonthDay;
        yearMonthDay[0] = year;
        yearMonthDay[1] = month + 1;
        yearMonthDay[2] = days + 1;

        return yearMonthDay;
    }

    long convertStringToLong(string str) {
        try {
            long temp = stol(str);
            return temp;
        }
        catch (...) {
            return -1;
        }
    }

    bool isSerial(string str) {
        for (int i = 0; i < str.length(); i++) {
            // Need to also potentially account for negatives. This method should really just be isNum, but all the same
            // Also need to account for decimals less than 1. we should only be dealing with decimals that are some variant of < 1, ex: .78 or 0.78
            if (!isdigit(str[i]) && (i != 0 && str[i] != '-'))
                return false;
        }

        return true;
    }

    bool isFloat(string str) {
        char* check;
        strtof(str.c_str(), &check);

        return *check == 0;
    }

    string convertDateString(string originalDateString) {
        string slashDelim = "/";
        string hyphenDelim = "-";
        string delimToUse = "";

        if (originalDateString.find(slashDelim) != string::npos)
            delimToUse = slashDelim;
        else if (originalDateString.find(hyphenDelim) != string::npos)
            delimToUse = hyphenDelim;
        // If we can't find either delimiter, I can't tell what they used so just return the same string back to where ever it was called.
        // Will probably cause issues, but the data is messed up anyway
        else
            return originalDateString;

        string dateMonth = originalDateString.substr(0, originalDateString.find(delimToUse));
        originalDateString.erase(0, originalDateString.find(delimToUse) + delimToUse.length());

        string dateDay = originalDateString.substr(0, originalDateString.find(delimToUse));
        originalDateString.erase(0, originalDateString.find(delimToUse) + delimToUse.length());

        string dateYear = originalDateString; 


        string temp = "";
        for (int i = 0; i < monthAbbrev.size(); i++) {
            if (monthAbbrev[i] == dateYear) {
                temp = dateMonth;
                dateMonth = dateYear;
                dateYear = temp;
                break;
            }
            else if (monthAbbrev[i] == dateDay) {
                temp = dateMonth;
                dateMonth = dateDay;
                dateDay = temp;
                break;
            }
        }

        // if the year is less than 4 digits long, that probably means they put it in as year/month/day
        // (or we're translating it using one of our own methods)
        if (dateYear.length() < 4) {
            string temp = "";
            temp = dateYear;
            dateYear = dateMonth;
            dateMonth = dateDay;
             dateDay = temp;
        }

        // If the month is greater than 12, we're going to assume that they put the month and day in opposite spots
        // Also, if the day length is 3, we're going to assume its some month abbreviation and replace it
        if ((dateMonth.length() < 3 && stoi(dateMonth) > 12) || dateDay.length() == 3) {
            string temp = "";
            temp = dateMonth;
            dateMonth = dateDay;
            dateDay = temp;
        }

        if (dateMonth.length() == 3) {

            string tempMonth = "";
            for (int i = 0; i < dateMonth.length(); i++) {
                tempMonth += toupper(dateMonth[i]);
            }

            dateMonth = tempMonth;

            for (int i = 0; i < monthAbbrev.size(); i++) {
                if (monthAbbrev[i] == dateMonth) {
                    dateMonth = to_string(i + 1);
                    break;
                }
            }
        }

        string newDateString = dateYear + "/" + dateMonth + "/" + dateDay;

        return newDateString;
    }

    array<int, 3> convertDateStringToIntArray(string dateString) {

        string newDateString = convertDateString(dateString);
        string delimToUse = "/";

        string dateYear = newDateString.substr(0, newDateString.find(delimToUse));
        newDateString.erase(0, newDateString.find(delimToUse) + delimToUse.length());

        string dateMonth = newDateString.substr(0, newDateString.find(delimToUse));
        newDateString.erase(0, newDateString.find(delimToUse) + delimToUse.length());

        string dateDay = newDateString;

        array <int, 3> yearMonthDay;
        yearMonthDay[0] = stoi(dateYear);
        yearMonthDay[1] = stoi(dateMonth);
        yearMonthDay[2] = stoi(dateDay);

        return yearMonthDay;
    }

    array<int, 3> standardizeTimeValue(string time_value) {

        array<int, 3> newTimeValue = { 0, 0, 0 };

        // detach the time value from the date value. if there isn't a time value we can use, just return 0.
        //  space as a delim works here because this should always split up our string into (optional)datestring/timevalue/amPM
        string delim = " ";

        array<string, 3> dateValues;
        string amPM = "";

        int i = 0;
        int delimPosition = 0;
        while (time_value != "" && delimPosition >= 0) {
            delimPosition = time_value.find(delim);
            dateValues[i] = time_value.substr(0, delimPosition);
            time_value.erase(0, delimPosition + delim.length());

            i++;
        }

        array<string, 3> splitTime;
        string timeDelim = ":";
        int timeDelimPos = 0;
        int dateValue0 = dateValues[0].find(timeDelim);
        int dateValue1 = dateValues[1].find(timeDelim);

        if (dateValue0 >= 0) {
            i = 0;
            while (dateValues[0] != "" && timeDelimPos >= 0) {
                timeDelimPos = dateValues[0].find(timeDelim);
                splitTime[i] = dateValues[0].substr(0, timeDelimPos);
                dateValues[0].erase(0, timeDelimPos + timeDelim.length());

                i++;
            }

            amPM = dateValues[1];
        }
        else if (dateValue1 >= 0) {
            string timeDelim = ":";
            int timeDelimPos = 0;
            i = 0;
            while (dateValues[1] != "" && timeDelimPos >= 0) {
                timeDelimPos = dateValues[1].find(timeDelim);
                splitTime[i] = dateValues[1].substr(0, timeDelimPos);
                dateValues[1].erase(0, timeDelimPos + timeDelim.length());

                i++;
            }

            amPM = dateValues[2];
        }
        else {
            // we can't determine a time value from what we expect to be given, so return the blank timevalue
            return newTimeValue;
        }

        // make sure we actually have numbers we're working with
        if (isSerial(splitTime[0]) && isSerial(splitTime[1])) {
            if (splitTime[2] != "" && isSerial(splitTime[2]))
                newTimeValue = { convertToMilitaryTime(stoi(splitTime[0]), amPM), stoi(splitTime[1]), stoi(splitTime[2]) };
            else
                newTimeValue = { convertToMilitaryTime(stoi(splitTime[0]), amPM), stoi(splitTime[1]), 00 };
        }



        return newTimeValue;
    }

    int convertToMilitaryTime(int hour, string ampm) {

        if ((ampm == "PM" || ampm == "pm") && hour != 12) {
            return hour + 12;
        }
        else if ((ampm == "AM" || ampm == "am") && hour == 12)
            return 0;

        return hour;
    }

    array<int, 3> getIsoWeekOne(int year) {
        array<int, 3> yearMonthDay = {year, 1, 4};

        int dayNumber = getDayOfTheWeek(yearMonthDay[0], yearMonthDay[1], yearMonthDay[2]);
        if (dayNumber == 0) {
            dayNumber = 7;
        }

        if ((1 - dayNumber) < 0 && (1 - dayNumber) < -3) {
            yearMonthDay[2] += (1 - dayNumber);
            yearMonthDay[2] += 31;


            yearMonthDay[0]--;

            yearMonthDay[1] = 12;
        }
        else {
            yearMonthDay[2] += (1 - dayNumber);
        }

        return yearMonthDay;
    }

    int getDayOfTheWeek(int year, int month, int day) {
        array<int,12>  t = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        year -= month < 3;
        return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
    }

    // return true if date1 is less than date2, otherwise false
    bool compareDates(array<int, 3> yearMonthDay1, array<int, 3> yearMonthDay2) {
        if (yearMonthDay1[0] < yearMonthDay2[0])
            return true;
        else if (yearMonthDay1[0] == yearMonthDay2[0] && yearMonthDay1[1] < yearMonthDay2[1])
            return true;
        else if (yearMonthDay1[0] == yearMonthDay2[0] && yearMonthDay1[1] == yearMonthDay2[1] && yearMonthDay1[2] < yearMonthDay2[2])
            return true;

        return false;
    }

    string convertDateArrayToString(array<int, 3> yearMonthDay) {
        return to_string(yearMonthDay[0]) + "/" + to_string(yearMonthDay[1]) + "/" + to_string(yearMonthDay[2]);
    }
};

