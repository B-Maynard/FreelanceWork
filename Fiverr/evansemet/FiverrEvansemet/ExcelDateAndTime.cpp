#include "Helpers.cpp"
#include <cmath>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <pybind11/chrono.h>
#include <vector>

using namespace std;
using namespace pybind11;
using sysclock_t = chrono::system_clock;

Helpers Helper;

// DATE function
string EXCEL_DATE(int year, int month, int day) {

    long serial = 0;

    if (month > 12) {
        year += (month / 12);
        month -= 12;
    }

    for (int i = 1900; i < year; i++) {
        if (Helper.isLeapYear(i) && i != 1900) serial += 366;
        else serial += 365;
    }

    if (month != 1) {
        if (Helper.isLeapYear(year)) serial += monthDaysTotalLeapYear[month - 2];
        else serial += monthDaysTotal[month - 2];
    }

    serial += day;

    return to_string(serial - 1);
}

// DATEDIF function
string EXCEL_DATEDIF(string start_date, string end_date, string unit) {
     string delimiter = "/";

     long start_date_serial = 0;
     long end_date_serial = 0;

     int endDateYearAsInt = 0;
     int startDateYearAsInt = 0;
     int endDateMonthAsInt = 0;
     int startDateMonthAsInt = 0;
     int endDateDayAsInt = 0;
     int startDateDayAsInt = 0;

     if (start_date.find("/") != string::npos) {

         startDateYearAsInt = 0;
         string startDateYear = start_date.substr(0, start_date.find(delimiter));
         startDateYearAsInt = stoi(startDateYear);
         start_date.erase(0, start_date.find(delimiter) + delimiter.length());

         startDateMonthAsInt = 0;
         string startDateMonth = start_date.substr(0, start_date.find(delimiter));
         startDateMonthAsInt = stoi(startDateMonth);
         start_date.erase(0, start_date.find(delimiter) + delimiter.length());

         startDateDayAsInt = 0;
         string startDateDay = start_date;
         startDateDayAsInt = stoi(startDateDay);

         string start_date_serial_string = EXCEL_DATE(startDateYearAsInt, startDateMonthAsInt, startDateDayAsInt);
         start_date_serial = stoi(start_date_serial_string);
     }
     else {
         // If we can't parse it as a long, return an error.
         long checkSerial = Helper.convertStringToLong(start_date);
         if (checkSerial == -1) return "#NUM!";

         array<int, 3> startDateStringArray = Helper.getDateFromSerial(start_date);

         startDateYearAsInt = startDateStringArray[0];
         startDateMonthAsInt = startDateStringArray[1];
         startDateDayAsInt = startDateStringArray[2];
         
     }


     if (end_date.find("/") != string::npos) {

         endDateYearAsInt = 0;
         string endDateYear = end_date.substr(0, end_date.find(delimiter));
         endDateYearAsInt = stoi(endDateYear);
         end_date.erase(0, end_date.find(delimiter) + delimiter.length());

         endDateMonthAsInt = 0;
         string endDateMonth = end_date.substr(0, end_date.find(delimiter));
         endDateMonthAsInt = stoi(endDateMonth);
         end_date.erase(0, end_date.find(delimiter) + delimiter.length());

         endDateDayAsInt = 0;
         string endDateDay = end_date;
         endDateDayAsInt = stoi(endDateDay);


         string end_date_serial_string = EXCEL_DATE(endDateYearAsInt, endDateMonthAsInt, endDateDayAsInt);
         end_date_serial = stoi(end_date_serial_string);
     }
     else {
         // If we can't parse it as a long, return an error.
         long checkSerial = Helper.convertStringToLong(end_date);
         if (checkSerial == -1) return "#NUM!";

         array<int, 3> endDateStringArray = Helper.getDateFromSerial(end_date);

         endDateYearAsInt = endDateStringArray[0];
         endDateMonthAsInt = endDateStringArray[1];
         endDateDayAsInt = endDateStringArray[2];
     }

     if (start_date_serial > end_date_serial) return "#NUM!";

     int yearDifference = endDateYearAsInt - startDateYearAsInt;
     int monthDifference = abs(endDateMonthAsInt - startDateMonthAsInt);
     int dayDifference = abs(endDateDayAsInt - startDateDayAsInt);

     if (unit == "Y" || unit == "y") {
         return to_string(yearDifference);
     }
     else if (unit == "M" || unit == "m") {
         return to_string(yearDifference * 12 + (endDateMonthAsInt - startDateMonthAsInt));
     }
     else if (unit == "D" || unit == "d") {
         return to_string(end_date_serial - start_date_serial);
     }
     else if (unit == "MD" || unit == "md") {
         return to_string(dayDifference);
     }
     else if (unit == "YM" || unit == "ym") {
         return to_string(monthDifference);
     }
     else if (unit == "YD" || unit == "yd") {
         // Get a temp start date value using the same year as the end date. That way we can just subtract the serial values.
         if (endDateYearAsInt > startDateYearAsInt) {
             int tempStartDateSerialAsInt = 0;
             string tempStartDateSerial = EXCEL_DATE(endDateYearAsInt, startDateMonthAsInt, startDateDayAsInt);
             tempStartDateSerialAsInt = stoi(tempStartDateSerial);

             return to_string(abs(end_date_serial - tempStartDateSerialAsInt));
         }
         // They can only ever be the same, or end date year be greater. Otherwise we would've errored already.
         else {
             return to_string(end_date_serial - start_date_serial);
         }
     }

}

// DATEVALUE function
string EXCEL_DATEVALUE(string date_text) {
    array<int, 3> yearMonthDay = Helper.convertDateStringToIntArray(date_text);

    return EXCEL_DATE(yearMonthDay[0], yearMonthDay[1], yearMonthDay[2]);
}

// DAY function
string EXCEL_DAY(string serial) {
    long serialAsLong = Helper.convertStringToLong(serial);
    if (serialAsLong == -1) return "#NUM!";

    array<int, 3> dateArray = Helper.getDateFromSerial(serial);
    return to_string(dateArray[2]);
}

// DAYS function
string EXCEL_DAYS(string end_date, string start_date) {

    if (!Helper.isSerial(end_date)) {
        array<int, 3> tempArray = Helper.convertDateStringToIntArray(end_date);
        end_date = EXCEL_DATE(tempArray[0], tempArray[1], tempArray[2]);
    }

    if (!Helper.isSerial(start_date)) {
        array<int, 3> tempArray = Helper.convertDateStringToIntArray(start_date);
        start_date = EXCEL_DATE(tempArray[0], tempArray[1], tempArray[2]);
    }

    return to_string(stoi(end_date) - stoi(start_date));
}

// DAYS360 function
string EXCEL_DAYS360(string start_date, string end_date, bool method = false) {
    // optional param "method" that validates whether or not we want to use the US method or the EU method

    if (!Helper.isSerial(start_date) || !Helper.isSerial(end_date)) return "#VALUE!";

    int dayCount = 0;

    array<int, 3> startDateYearMonthDay = Helper.getDateFromSerial(start_date);
    array<int, 3> endDateYearMonthDay = Helper.getDateFromSerial(end_date);
    bool isOpposite = false;

    if (startDateYearMonthDay[0] > endDateYearMonthDay[0]) {
        array<int, 3> temp = startDateYearMonthDay;
        endDateYearMonthDay = startDateYearMonthDay;
        startDateYearMonthDay = temp;
        isOpposite = true;
    }

    // re-assign the correct month/days/year for both the start and end dates
    if (!method) {
        if (Helper.isLeapYear(startDateYearMonthDay[0])) {
            // if the day is the last day of whatever month we're currently in, 
            if (startDateYearMonthDay[2] == monthDaysLeapYear[startDateYearMonthDay[1] - 1]) {
                startDateYearMonthDay[2] = 30;
            }
        }
        else {
            if (startDateYearMonthDay[2] == monthDays[startDateYearMonthDay[1] - 1]) {
                startDateYearMonthDay[2] = 30;
            }
        }

        if (Helper.isLeapYear(endDateYearMonthDay[0])) {
            if (endDateYearMonthDay[2] == monthDaysLeapYear[endDateYearMonthDay[1] - 1] && startDateYearMonthDay[2] < 30) {
                // if its the last day of the year, we need to advance to the next year
                if (endDateYearMonthDay[1] == 12) {
                    endDateYearMonthDay[0]++;
                    endDateYearMonthDay[1] = 1;
                    endDateYearMonthDay[2] = 1;
                }
                // Otherwise, just set it equal to the first day of the next month
                else {
                    endDateYearMonthDay[1]++;
                    endDateYearMonthDay[2] = 1;
                }
            }
            else if (endDateYearMonthDay[2] == monthDaysLeapYear[endDateYearMonthDay[1] - 1]) {
                endDateYearMonthDay[2] = 30;
            }
        }
        else {
            if (endDateYearMonthDay[2] == monthDays[endDateYearMonthDay[1] - 1] && startDateYearMonthDay[2] < 30) {
                // if its the last day of the year, we need to advance to the next year
                if (endDateYearMonthDay[1] == 12) {
                    endDateYearMonthDay[0]++;
                    endDateYearMonthDay[1] = 1;
                    endDateYearMonthDay[2] = 1;
                }
                // Otherwise, just set it equal to the first day of the next month
                else {
                    endDateYearMonthDay[1]++;
                    endDateYearMonthDay[2] = 1;
                }
            }
            else if (endDateYearMonthDay[2] == monthDays[endDateYearMonthDay[1] - 1]) {
                endDateYearMonthDay[2] = 30;
            }
        }
    }
    else {
        if (Helper.isLeapYear(startDateYearMonthDay[0]) && startDateYearMonthDay[2] == monthDaysLeapYear[startDateYearMonthDay[1] - 1]) {
            startDateYearMonthDay[2] = 30;
        }
        else if (startDateYearMonthDay[2] == monthDays[startDateYearMonthDay[1] - 1]) {
            startDateYearMonthDay[2] = 30;
        }

        if (Helper.isLeapYear(endDateYearMonthDay[0]) && endDateYearMonthDay[2] == monthDaysLeapYear[endDateYearMonthDay[1] - 1]) {
            endDateYearMonthDay[2] = 30;
        }
        else if (endDateYearMonthDay[2] == monthDays[endDateYearMonthDay[1] - 1]) {
            endDateYearMonthDay[2] = 30;
        }
    }


    dayCount += (30 - startDateYearMonthDay[2]);
    dayCount += endDateYearMonthDay[2];
    dayCount += ((endDateYearMonthDay[0] - startDateYearMonthDay[0]) - 1) * 360;

    dayCount += (12 - startDateYearMonthDay[1]) * 30;
    dayCount += (endDateYearMonthDay[1] - 1) * 30;

    if (isOpposite) return to_string(dayCount * -1);

    return to_string(dayCount);
    
}

// EDATE function
string EXCEL_EDATE(string start_date, string months) {

    if (!Helper.isSerial(start_date) || !Helper.isSerial(months)) return "#VALUE!";

    int startDateAsSerial = stoi(start_date);
    int monthsAsInt = stoi(months);

    array<int, 3> yearMonthDay = Helper.getDateFromSerial(start_date);
    yearMonthDay[1] += monthsAsInt;

    if (yearMonthDay[1] > 12) {
        while (yearMonthDay[1] > 12) {
            yearMonthDay[0] += 1;
            yearMonthDay[1] -= 12;
        }
    }
    else if (yearMonthDay[1] < 12) {
        while (yearMonthDay[1] < 0) {
            yearMonthDay[0] -= 1;
            yearMonthDay[1] += 12;
        }
    }

        
    return EXCEL_DATE(yearMonthDay[0], yearMonthDay[1], yearMonthDay[2]);
}

// EOMONTH function
string EXCEL_EOMONTH(string start_date, string months) {

    if (!Helper.isSerial(start_date) || !Helper.isSerial(months)) return "#VALUE!";

    string tempSerial = EXCEL_EDATE(start_date, months);

    array<int, 3> yearMonthDay = Helper.getDateFromSerial(tempSerial);

    if (Helper.isLeapYear(yearMonthDay[0]))
        yearMonthDay[2] = monthDaysLeapYear[yearMonthDay[1] - 1];
    else
        yearMonthDay[2] = monthDays[yearMonthDay[1] - 1];

    return EXCEL_DATE(yearMonthDay[0], yearMonthDay[1], yearMonthDay[2]);
}

// HOUR function
string EXCEL_HOUR(string serial_number) {
    if (Helper.isFloat(serial_number)) {
        float hour = 0;
        float floatValue = (float)24 * stof(serial_number);

        modf(floatValue, &hour);

        return to_string((int)hour);
    }
    else {
        array<int, 3> timeValues = Helper.standardizeTimeValue(serial_number);
        return to_string(timeValues[0]);
    }
}

// ISOWEKEKNUM function
string EXCEL_ISOWEEKNUM(string date) {

    // if neither work, then just return that it isn't a valid date string cause something is messed up
    if (!Helper.isSerial(date) && ((Helper.convertDateString(date)) == date)) return "#VALUE!";

    // if its not a serial but it could be converted to a date string, convert it instead to a serial
    if (!Helper.isSerial(date)) {
        array<int, 3> temp = Helper.convertDateStringToIntArray(date);
        date = EXCEL_DATE(temp[0], temp[1], temp[2]);
    }

    array<int, 3> yearMonthDay = Helper.getDateFromSerial(date);

    int december = 12;
    int decemberDay = 29;

    int isoYear = yearMonthDay[0];
        
    array<int, 3> week1;
    // we should only really be concerned with 12/29, 12/30, and 12/31 for this accomodation, since everything else will either be less than these or into the next year.
    if (yearMonthDay[1] == december && yearMonthDay[2] >= decemberDay) {
        week1 = Helper.getIsoWeekOne(isoYear + 1);
        if (Helper.compareDates(yearMonthDay, week1)) {
            week1 = Helper.getIsoWeekOne(isoYear);
        }
        else {
            isoYear++;
        }
    }
    else {
        week1 = Helper.getIsoWeekOne(isoYear);
        if (Helper.compareDates(yearMonthDay, week1)) {
            week1 = Helper.getIsoWeekOne(--isoYear);
        }
    }

    string dtString = Helper.convertDateArrayToString(yearMonthDay);
    string week1String = Helper.convertDateArrayToString(week1);

    int isoWeek = ((isoYear * 100) + (stoi(EXCEL_DAYS(dtString, week1String))) / 7 + 1);

    return to_string(isoWeek % 100);
}

// MINUTE function
string EXCEL_MINUTE(string serial_number) {
    if (Helper.isFloat(serial_number)) {
        float hour = 0;
        float floatValue = (float)24 * stof(serial_number);

        modf(floatValue, &hour);
        floatValue -= hour;


        return to_string((int)(floatValue * 60));
    }
    else {
        array<int, 3> timeValues = Helper.standardizeTimeValue(serial_number);
        return to_string(timeValues[1]);
    }
}

// MONTH function
string EXCEL_MONTH(string serial_number) {

    if (!Helper.isSerial(serial_number)) return "#VALUE!";

    array<int, 3> yearMonthDay = Helper.getDateFromSerial(serial_number);

    return to_string(yearMonthDay[1]);
}

// NETWORKDAYS function
string EXCEL_NETWORKDAYS(string start_date, string end_date, vector<string> holidays = {}) {

    // if neither work, then just return that it isn't a valid date string cause something is messed up
    if (!Helper.isSerial(start_date) && ((Helper.convertDateString(start_date)) == start_date)) return "#VALUE!";

    // if neither work, then just return that it isn't a valid date string cause something is messed up
    if (!Helper.isSerial(end_date) && ((Helper.convertDateString(end_date)) == end_date)) return "#VALUE!";

    array<int, 3> startDateYMD;
    array<int, 3> endDateYMD;

    // if its not a serial but it could be converted to a date string, convert it instead to a serial
    if (!Helper.isSerial(start_date)) {
        startDateYMD = Helper.convertDateStringToIntArray(start_date);
        start_date = EXCEL_DATE(startDateYMD[0], startDateYMD[1], startDateYMD[2]);
    }
    else {
        startDateYMD = Helper.getDateFromSerial(start_date);
    }

    if (!Helper.isSerial(end_date)) {
        endDateYMD = Helper.convertDateStringToIntArray(end_date);
        end_date = EXCEL_DATE(endDateYMD[0], endDateYMD[1], endDateYMD[2]);
    }
    else {
        endDateYMD = Helper.getDateFromSerial(end_date);
    }

    // should only be dealing with serials from here

    int daysBetween = stoi(EXCEL_DAYS(end_date, start_date));

    int endDateDOW = Helper.getDayOfTheWeek(endDateYMD[0], endDateYMD[1], endDateYMD[2]);
    int startDateDOW = Helper.getDayOfTheWeek(startDateYMD[0], startDateYMD[1], startDateYMD[2]);

    int businessDays = 1 + ((daysBetween * 5 - ((startDateDOW - endDateDOW) * 2)) / 7);
    if (endDateDOW == 7) businessDays--;
    if (startDateDOW == 0) businessDays--;

    for (string holiday : holidays) {

        array<int, 3> tempYMD;
        if (!Helper.isSerial(holiday))
            tempYMD = Helper.convertDateStringToIntArray(holiday);
        else
            tempYMD = Helper.getDateFromSerial(holiday);

        // if the holiday is greater than the start date but less than the end date, that means it happens between so we need to decrement the business days
        if (!Helper.compareDates(tempYMD, startDateYMD) && Helper.compareDates(tempYMD, endDateYMD)) businessDays--;
    }


    return to_string(businessDays);
}\

// NETWORKINTL function
string EXCEL_NETWORKDAYSINTL(string start_date, string end_date, string weekend = "1", vector<string> holidays = {}) {

    if ((!Helper.isSerial(start_date) && ((Helper.convertDateString(start_date)) == start_date)) || (!Helper.isSerial(end_date) && ((Helper.convertDateString(end_date)) == end_date))) return "#VALUE!";

    if (!Helper.isSerial(weekend)) return "#NUM!";

    int startDateAsSerial = 0;
    int endDateAsSerial = 0;
    array<int, 3> ymdStartDate = {};
    array<int, 3> ymdEndDate = {};

    if (Helper.isSerial(start_date)) {
        startDateAsSerial = stoi(start_date);
        ymdStartDate = Helper.getDateFromSerial(start_date);
    }
    else {
        ymdStartDate = Helper.convertDateStringToIntArray(start_date);
        startDateAsSerial = stoi(EXCEL_DATE(ymdStartDate[0], ymdStartDate[1], ymdStartDate[2]));
    }

    if (Helper.isSerial(end_date)) {
        endDateAsSerial = stoi(end_date);
        ymdEndDate = Helper.getDateFromSerial(end_date);
    }
    else {
        ymdEndDate = Helper.convertDateStringToIntArray(end_date);
        endDateAsSerial = stoi(EXCEL_DATE(ymdEndDate[0], ymdEndDate[1], ymdEndDate[2]));
    }

    int weekendAsInt = stoi(weekend);
    int startDateDOW = Helper.getDayOfTheWeek(ymdStartDate[0], ymdStartDate[1], ymdStartDate[2]);
    int endDateDOW = Helper.getDayOfTheWeek(ymdEndDate[0], ymdEndDate[1], ymdEndDate[2]);

    int daysToUse = 0;
    int daysBetween = stoi(EXCEL_DAYS(end_date, start_date));
    int daysToAccountFor = 0;

    if (weekendAsInt == 1) {
        return EXCEL_NETWORKDAYS(start_date, end_date, holidays);
    }
    else if (weekendAsInt == 2) {
        if (startDateDOW == 0)
            daysToAccountFor++;
        if (startDateDOW == 1)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 3) {
        if (startDateDOW == 1)
            daysToAccountFor++;
        if (startDateDOW == 2)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 4) {
        if (startDateDOW == 2)
            daysToAccountFor++;
        if (startDateDOW == 3)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 5) {
        if (startDateDOW == 3)
            daysToAccountFor++;
        if (startDateDOW == 4)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 6) {
        if (startDateDOW == 4)
            daysToAccountFor++;
        if (startDateDOW == 5)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 7) {
        if (startDateDOW == 5)
            daysToAccountFor++;
        if (startDateDOW == 6)
            daysToAccountFor++;

        daysToUse = 5;
    }
    else if (weekendAsInt == 11) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (endDateDOW == 0) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 12) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 1 || endDateDOW == 1) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 13) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 2 || endDateDOW == 2) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 14) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 3 || endDateDOW == 3) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 15) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 4 || endDateDOW == 4) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 16) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 5 || endDateDOW == 5) daysToAccountFor++;

        daysToUse = 6;
    }
    else if (weekendAsInt == 17) {

        daysToAccountFor += ((float)daysBetween / 6);

        if (startDateDOW <= 6 || endDateDOW == 6) daysToAccountFor++;

        daysToUse = 6;
    }
    else {
        vector<int> weekendVector = {};

        if (weekend.size() != 7) return "#NUM!";
        if (weekend == "1111111") return "0";

        for (char item : weekend) {
            if (item != '0' && item != '1') return "#NUM!";
            else weekendVector.push_back((item - '0'));
        }

        int i = 0;
        for (int num : weekendVector) {
            if (startDateDOW <= i && num == 1 && i != 0 && i != 6)
                daysToAccountFor++;

            if (endDateDOW >= i && num == 1 && i != 0 && i != 6)
                daysToAccountFor++;

            if (num == 1) {
                daysToUse++;
            }

            i++;
        }

        daysToUse = 7 - daysToUse;
    }

    int businessDays = 1 + ((daysBetween * daysToUse - ((startDateDOW - endDateDOW) * (7 - daysToUse))) / 7);
    businessDays -= daysToAccountFor;

    for (string holiday : holidays) {

        array<int, 3> tempYMD;
        if (!Helper.isSerial(holiday))
            tempYMD = Helper.convertDateStringToIntArray(holiday);
        else
            tempYMD = Helper.getDateFromSerial(holiday);

        // if the holiday is greater than the start date but less than the end date, that means it happens between so we need to decrement the business days
        if (!Helper.compareDates(tempYMD, ymdStartDate) && Helper.compareDates(tempYMD, ymdEndDate)) businessDays--;
    }

    return to_string(businessDays);
}

// NOW function
string EXCEL_NOW() {
       
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

    char buf[17] = { 0 };
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    std::strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M", &timeinfo);

    string timeString = string(buf);


    return timeString;
}

// SECOND function
string EXCEL_SECOND(string serial_number) {
    if (Helper.isFloat(serial_number)) {
        float hour = 0;
        float minute = 0;
        float floatValue = (float)24 * stof(serial_number);

        modf(floatValue, &hour);
        floatValue -= hour;

        floatValue *= 60;

        modf(floatValue, &minute);
        floatValue -= minute;

        return to_string((int)(floatValue * 60));
    }
    else {
        array<int, 3> timeValues = Helper.standardizeTimeValue(serial_number);
        return to_string(timeValues[2]);
    }
}

// TIMEVALUE function
string EXCEL_TIMEVALUE(string time_text) {

    if (Helper.isFloat(time_text)) return time_text;

    array<int, 3> timeValues = Helper.standardizeTimeValue(time_text);

    float value = 0;

    value += (float)timeValues[0] / (float)24;
    value += (float)timeValues[1] / (float)(60 * 24);
    value += (float)timeValues[2] / (float)(60 * 60 * 24);


    return to_string(value);
}

// TIME function
string EXCEL_TIME(string hour, string minute, string second) {

    if (!Helper.isSerial(hour) || !Helper.isSerial(minute) || !Helper.isSerial(second)) return "#NUM!";

    if (stol(hour) > 32767)
        hour = "32767";
    else if (stol(hour) < 0)
        hour = "0";

    if (stol(minute) > 32767)
        minute = "32767";
    else if (stol(minute) < 0)
        minute = "0";

    if (stol(second) > 32767)
        second = "32767";
    else if (stol(second) < 0)
        second = "0";

    int hourInt = stoi(hour);
    int minuteInt = stoi(minute);
    int secondInt = stoi(second);

    int newSeconds = secondInt % 60;
    secondInt = secondInt - (newSeconds);
    minuteInt += (secondInt / 60);
    secondInt = newSeconds;

    int newMinutes = minuteInt % 60;
    minuteInt = minuteInt - (newMinutes);
    hourInt += (minuteInt / 60);
    minuteInt = newMinutes;

    if (hourInt > 23)
        hourInt = hourInt % 24;

    string timeValue = "";
    if (minuteInt < 10)
        timeValue = to_string(hourInt) + ":0" + to_string(minuteInt);

    if (secondInt < 10)
        timeValue += ":0" + to_string(secondInt);
    else
        timeValue += ":" + to_string(secondInt);


    return EXCEL_TIMEVALUE(timeValue);
}

// TODAY function
string EXCEL_TODAY() {
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

    char buf[17] = { 0 };
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    std::strftime(buf, sizeof(buf), "%Y/%m/%d", &timeinfo);

    string timeString = string(buf);
    return timeString;
}

// WEEKDAY function
string EXCEL_WEEKDAY(string serial_number, string return_type = "1") {

    if (!Helper.isSerial(serial_number)) return "#NUM!";
    
    if (!Helper.isSerial(return_type) || ((stoi(return_type) < 1) && (stoi(return_type) > 17))) return "#NUM!";

    int returnTypeAsInt = stoi(return_type);

    array<int, 3> ymd = Helper.getDateFromSerial(serial_number);

    //getDayOfWeek returns 0-6, sunday-saturday

    int day = 0;
    switch (returnTypeAsInt) {
    case 1:
        return to_string((Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2])) + 1);
    case 2:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;

        return to_string(day);
    case 3:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;

        day--;

        return to_string(day);
    case 11:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;

        return to_string(day);
    case 12:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;
        if (day == 1) day = 8;

        day--;

        return to_string(day);
    case 13:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;
        if (day == 1) day = 8;
        if (day == 2) day = 9;

        day -= 2;

        return to_string(day);
    case 14:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;
        if (day == 1) day = 8;
        if (day == 2) day = 9;
        if (day == 3) day = 10;

        day -= 3;

        return to_string(day);
    case 15:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;
        if (day == 1) day = 8;
        if (day == 2) day = 9;
        if (day == 3) day = 10;
        if (day == 4) day = 11;

        day -= 4;

        return to_string(day);
    case 16:
        day = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);
        if (day == 0) day = 7;
        if (day == 1) day = 8;
        if (day == 2) day = 9;
        if (day == 3) day = 10;
        if (day == 4) day = 11;
        if (day == 5) day = 12;

        day -= 5;

        return to_string(day);
    case 17:
        return to_string((Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2])) + 1);
    default:
        return "#VALUE!";
    }
}

// WEEKNUM function
string EXCEL_WEEKNUM(string serial_number, string return_type = "1") {

    if (!Helper.isSerial(serial_number)) return "#NUM!";

    if (!Helper.isSerial(return_type) || ((stoi(return_type) < 1) && (stoi(return_type) > 21))) return "#NUM!";

    int returnTypeAsInt = stoi(return_type);

    array<int, 3> ymd = Helper.getDateFromSerial(serial_number);

    string isoWeekNum = EXCEL_ISOWEEKNUM(serial_number);
    int weekNumAsInt = stoi(isoWeekNum);

    int dayOfWeek = Helper.getDayOfTheWeek(ymd[0], ymd[1], ymd[2]);

    if (returnTypeAsInt == 1 || returnTypeAsInt == 17) {
        // if we're on a sunday when using iso week num, then we'll be on the week before, since new weeks start on mondays for iso weeks. just add one in this case, since it would techinically be the start
        // of the next week
        if (dayOfWeek == 0) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 2 || returnTypeAsInt == 11) {
        // if our day is anytime past/including monday, then it would techincally be the next week so add 1
        if (dayOfWeek >= 1) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 12) {
        if (dayOfWeek >= 2) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 13) {
        if (dayOfWeek >= 3) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 14) {
        if (dayOfWeek >= 4) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 15) {
        if (dayOfWeek >= 5) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 16) {
        if (dayOfWeek >= 6) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else if (returnTypeAsInt == 21) {
        if (Helper.getDayOfTheWeek(ymd[0], 1, 1) > 4) weekNumAsInt--;

        if (dayOfWeek == 0) return to_string(weekNumAsInt + 1);
        else return isoWeekNum;
    }
    else {
        return "#VALUE!";
    }
}

// WORKDAY function
string EXCEL_WORKDAY(string start_date, string days, vector<string> holidays = {}) {

    int startDateAsSerial = 0;

    if ((!Helper.isSerial(start_date) && (Helper.convertDateString(start_date) == start_date)) || (!Helper.isSerial(days)))
        return "#NUM!";
    else if (!Helper.isSerial(start_date)) {
        array<int, 3> temp = Helper.convertDateStringToIntArray(start_date);
        start_date = EXCEL_DATE(temp[0], temp[1], temp[2]);
    }

    startDateAsSerial = Helper.convertStringToLong(start_date);

    // weeks if split up by 5 days, and then time 2 days per each weeks
    int newDays = (stoi(days) / 5) * 2;
    int endDateAsSerial = startDateAsSerial + newDays + stoi(days);

    array<int, 3> ymdStartDate = Helper.getDateFromSerial(to_string(startDateAsSerial));
    array<int, 3> ymdEndDate = Helper.getDateFromSerial(to_string(endDateAsSerial));

    int holidaysToCount = 0;
    for (string holiday : holidays) {

        array<int, 3> tempYMD;
        if (!Helper.isSerial(holiday))
            tempYMD = Helper.convertDateStringToIntArray(holiday);
        else
            tempYMD = Helper.getDateFromSerial(holiday);

        // if the holiday is greater than the start date but less than the end date, that means it happens between so we need to decrement the business days
        if (!Helper.compareDates(tempYMD, ymdStartDate) && Helper.compareDates(tempYMD, ymdEndDate)) holidaysToCount++;
    }

    endDateAsSerial += holidaysToCount;

    // need to see if anymore weekends would be included after the holiday additions, so redundant but necessary for accurate dates
    endDateAsSerial += round((float)(((stof(days) + (float)holidaysToCount) / 5) * 2) - (float)newDays);

    ymdEndDate = Helper.getDateFromSerial(to_string(endDateAsSerial));

    return EXCEL_DATE(ymdEndDate[0], ymdEndDate[1], ymdEndDate[2]);
}

// WORKDAYINTL function
string EXCEL_WORKDAYINTL(string start_date, string days, string weekend = "1", vector<string> holidays = {}) {

    if ((!Helper.isSerial(start_date) && ((Helper.convertDateString(start_date)) == start_date))) return "#VALUE!";

    if (!Helper.isSerial(weekend)) return "#NUM!";
    if (!Helper.isSerial(days)) return "#NUM!";

    int startDateAsSerial = 0;
    array<int, 3> ymdStartDate = {};
    array<int, 3> ymdEndDate = {};

    if (Helper.isSerial(start_date)) {
        startDateAsSerial = stoi(start_date);
        ymdStartDate = Helper.getDateFromSerial(start_date);
    }
    else {
        ymdStartDate = Helper.convertDateStringToIntArray(start_date);
        startDateAsSerial = stoi(EXCEL_DATE(ymdStartDate[0], ymdStartDate[1], ymdStartDate[2]));
    }

    int weekendDays = 0;

    int newDays = 0;
    int weekendAsInt = stoi(weekend);

    int endDateAsSerial = 0;
    int startDateDOW = Helper.getDayOfTheWeek(ymdStartDate[0], ymdStartDate[1], ymdStartDate[2]);

    int daysToUse = 0;

    if (weekendAsInt == 1) {
        return EXCEL_WORKDAY(start_date, days, holidays);
    }
    else if (weekendAsInt == 2) {
        if (startDateDOW == 0)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 3) {
        if (startDateDOW == 1)
            endDateAsSerial += 2;
        else if (startDateDOW == 2)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 4) {
        if (startDateDOW == 2)
            endDateAsSerial += 2;
        else if (startDateDOW == 3)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 5) {
        if (startDateDOW == 3)
            endDateAsSerial += 2;
        else if (startDateDOW == 4)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 6) {
        if (startDateDOW == 4)
            endDateAsSerial += 2;
        else if (startDateDOW == 5)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 7) {
        if (startDateDOW == 5)
            endDateAsSerial += 2;
        else if (startDateDOW == 6)
            endDateAsSerial++;

        newDays = (stoi(days) / 5) * 2;
        daysToUse = 5;
    }
    else if (weekendAsInt == 11) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 0) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 12) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 1) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 13) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 2) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 14) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 3) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 15) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 4) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 16) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 5) endDateAsSerial++;
        daysToUse = 6;
    }
    else if (weekendAsInt == 17) {
        newDays = (float)(stof(days) / 6);
        if (startDateDOW == 6) endDateAsSerial++;
        daysToUse = 6;
    }
    else {
        vector<int> weekendVector = {};

        if (weekend.size() != 7) return "#NUM!";
        if (weekend == "1111111") return "#NUM!";

        for (char item : weekend) {
            if (item != '0' && item != '1') return "#NUM!";
            else weekendVector.push_back((item - '0'));
        }

        int i = 0;
        for (int num : weekendVector) {
            if (startDateDOW <= i && num == 1) {
                endDateAsSerial++;
            }

            if (num == 1) {
                daysToUse++;
            }

            i++;
        }
    }

    endDateAsSerial += startDateAsSerial + newDays + stoi(days);
    ymdEndDate = Helper.getDateFromSerial(to_string(endDateAsSerial));

    int holidaysToCount = 0;
    for (string holiday : holidays) {

        array<int, 3> tempYMD;
        if (!Helper.isSerial(holiday))
            tempYMD = Helper.convertDateStringToIntArray(holiday);
        else
            tempYMD = Helper.getDateFromSerial(holiday);

        // if the holiday is greater than the start date but less than the end date, that means it happens between so we need to decrement the business days
        if (!Helper.compareDates(tempYMD, ymdStartDate) && Helper.compareDates(tempYMD, ymdEndDate)) holidaysToCount++;
    }

    endDateAsSerial += holidaysToCount;

    // need to see if anymore weekends would be included after the holiday additions, so redundant but necessary for accurate dates
    endDateAsSerial += round((float)(((stof(days) + (float)holidaysToCount) / daysToUse) * (7 - daysToUse)) - (float)newDays);

    ymdEndDate = Helper.getDateFromSerial(to_string(endDateAsSerial));

    return to_string(endDateAsSerial);
}

// YEAR function
string EXCEL_YEAR(string serial_number) {
    if (!Helper.isSerial(serial_number)) return "#VALUE!";

    array<int, 3> yearMonthDay = Helper.getDateFromSerial(serial_number);

    return to_string(yearMonthDay[0]);
}

// YEARFRAC function
string EXCEL_YEARFRAC(string start_date, string end_date, string basis = "0") {

    if (!Helper.isSerial(start_date) || !Helper.isSerial(end_date) || !Helper.isSerial(basis)) return "#NUM!";

    int startDateAsInt = stoi(start_date);
    int endDateAsInt = stoi(end_date);
    int basisAsInt = stoi(basis);

    if (basisAsInt < 0 || basisAsInt > 4) return "#NUM!";

    return "";
}

PYBIND11_MODULE(ExcelDateAndTime, m) {
    //DATE
    m.def("DATE", &EXCEL_DATE, R"pbdoc(
        
    )pbdoc");

    //DATEDIF
    m.def("DATEDIF", &EXCEL_DATEDIF, R"pbdoc(
        
    )pbdoc");

    // DATEVALUE
    m.def("DATEVALUE", &EXCEL_DATEVALUE, R"pbdoc(
        
    )pbdoc");

    //DAY
    m.def("DAY", &EXCEL_DAY, R"pbdoc(
        
    )pbdoc");

    //DAYS
    m.def("DAYS", &EXCEL_DAYS, R"pbdoc(
        
    )pbdoc");

    //DAYS360
    m.def("DAYS360", &EXCEL_DAYS360, R"pbdoc(
        
    )pbdoc");

    //EDATE
    m.def("EDATE", &EXCEL_EDATE, R"pbdoc(
        
    )pbdoc");

    //EOMONTH
    m.def("EOMONTH", &EXCEL_EOMONTH, R"pbdoc(
        
    )pbdoc");

    //HOUR
    m.def("HOUR", &EXCEL_HOUR, R"pbdoc(
        
    )pbdoc");

    //ISOWEEKNUM
    m.def("ISOWEEKNUM", &EXCEL_ISOWEEKNUM, R"pbdoc(
        
    )pbdoc");

    //MINUTE
    m.def("MINUTE", &EXCEL_MINUTE, R"pbdoc(
        
    )pbdoc");

    //MONTH
    m.def("MONTH", &EXCEL_MONTH, R"pbdoc(
        
    )pbdoc");

    //NETWORKDAYS
    m.def("NETWORKDAYS", &EXCEL_NETWORKDAYS, R"pbdoc(
        
    )pbdoc");

    //NOW
    m.def("NOW", &EXCEL_NOW, R"pbdoc(
        
    )pbdoc");

    //SECOND
    m.def("SECOND", &EXCEL_SECOND, R"pbdoc(
        
    )pbdoc");

    //TIME
    m.def("TIME", &EXCEL_TIME, R"pbdoc(
        
    )pbdoc");

    //TIMEVALUE
    m.def("TIMEVALUE", &EXCEL_TIMEVALUE, R"pbdoc(
        
    )pbdoc");

    //TODAY
    m.def("TODAY", &EXCEL_TODAY, R"pbdoc(
        
    )pbdoc");

    //WEEKDAY
    m.def("WEEKDAY", &EXCEL_WEEKDAY, R"pbdoc(
        
    )pbdoc");

    //WEEKNUM
    m.def("WEEKNUM", &EXCEL_WEEKNUM, R"pbdoc(
        
    )pbdoc");

    //MONTH
    m.def("WORKDAY", &EXCEL_WORKDAY, R"pbdoc(
        
    )pbdoc");

    //MONTH
    m.def("WORKDAYINTL", &EXCEL_WORKDAYINTL, R"pbdoc(
        
    )pbdoc");

    //YEAR
    m.def("YEAR", &EXCEL_YEAR, R"pbdoc(
        
    )pbdoc");

    //YEARFRAC
    m.def("YEARFRAC", &EXCEL_YEARFRAC, R"pbdoc(
        
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}


