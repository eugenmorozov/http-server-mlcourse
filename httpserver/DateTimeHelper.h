#ifndef HTTP_SERVER_MLCOURSE_DATETIMEHELPER_H
#define HTTP_SERVER_MLCOURSE_DATETIMEHELPER_H
#include <cstring>
#include <iostream>
#include <assert.h>
#include <sstream>

class DateTimeHelper {
public:
    static std::string getDayName(int dayNumber) {
        assert(dayNumber < 7);
        static std::string wdayName[7] =
                {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

        return wdayName[dayNumber - 1];
    }

    static std::string getMonthName(int monthNumber) {
        assert(monthNumber < 13 && monthNumber > 0);
        static std::string monthName[12] =
                {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

        return monthName[monthNumber - 1];
    }

    static std::string getDateHeader() {
        const time_t timer = time(nullptr);
        struct tm *now = localtime(&timer);
        std::stringstream timeHeader;
        char time[10];

        strftime(time, 10, "%H:%M:%S", now);

        timeHeader << getDayName(now->tm_wday) << ", ";
        if (now->tm_mday < 10) {
            timeHeader << "0" << now->tm_mday << " ";
        } else {
            timeHeader << now->tm_mday << " ";
        }
        timeHeader << getMonthName(now->tm_mon + 1) << " " << now->tm_year + 1900 << " "
                   << time << " " << "GMT";

        return timeHeader.str();
    }
};


#endif //HTTP_SERVER_MLCOURSE_DATETIMEHELPER_H
