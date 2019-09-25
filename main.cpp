#include <iostream>

// Student name: Tomas Vosylius / 1042957

enum Month
{
    January = 1, February, March, April, May, June,
    July, August, September, October, November, December
};




int easter_base(int year)
{
    /*  The function easter_base uses the Meeus/Jones/Butcher formula to compute
        the number that is used to determine on which month (easter_month below)
        and day (easter_day below) Easter is in the given year.
    */
    const int A     = year % 19 ;
    const int B     = year / 100 ;
    const int C     = year % 100 ;
    const int D     = B / 4 ;
    const int E     = B % 4 ;
    const int F     = (B + 8) / 25 ;
    const int G     = (B - F + 1) / 3 ;
    const int H     = (19 * A + B - D - G + 15) % 30 ;
    const int I     = C / 4 ;
    const int K     = C % 4 ;
    const int L     = (32 + 2 * E + 2 * I - H - K) % 7 ;
    const int M     = (A + 11 * H + 22 * L) / 451 ;
    return H + L - 7 * M + 114 ;
}

int easter_day(int year)
{
    /*  The result of easter_year is the day number of the month in which Easter occurs in
        the given year.
    */
    return (easter_base(year) % 31) + 1;
}

Month easter_month (int year)
{
    /*  The result of easter_month is the month in which Easter occurs in the given year.
    */
    return static_cast<Month>(easter_base(year) / 31);
}


/********************************************************************
    Assignment part 1: Leap years
********************************************************************/
bool is_leap_year(int year)
{
    /*  If number is divisible by 100, it has to be divisible by 400 too.
        If number is divisible by 4, it can not be divisible by 100 at the same time.;
    */
    return ((year % 400) == 0 || ( (year % 4) == 0 && (year % 100) != 0));
}

int number_of_days_in_month(int year, Month month)
{
    int days;

    if(month == February) days = (is_leap_year(year) ? 29 : 28);
    else if(month == 4 || month == 6 || month == 9 || month == 11) days = 30;
    else days = 31;

    return days;
}
/********************************************************************
    Assignment part 2: Holy days based on Easter
********************************************************************/
void show_holy_day(std::string name, int startYear, int startMonth, int startDay, int difference)
{
    /*  Function will return base of new holy day date, given start date and difference(in days):
        Return form: month*31 + days

        So it can be later extracted with:
        month = base / 31
        day = (base % 31) + 1
    */
    int newMonth    = startMonth,
        newDay      = startDay;
    bool adding     = (difference > 0); // If difference is positive, we are adding days. Otherwise, we are subtracting.

    /*  We know whether we are subtracting or adding, so we can invert negative to positive now.
        We will need for while loop
    */
    if(difference < 0) difference *= -1;

    while(difference --> 0)
    {
        if(!adding)
        {
            /*  If difference is negative, we are subtracting days (from Easter date in this assignment)
            */
            newDay--;
            if(newDay <= 0)
            {
                // We subtracted entire month (or what has left of it), so move to (month-1), and start from last day of that month.
                newMonth -- ;
                /*  We could check if(newMonth <= 0) and subtract 1 from current year in loop (also would be newMonth = 12)
                    But we don't have to take care of this, since all holy days are on the same year.
                    It is also impossible for year to change, because Easter is in spring and max difference is -49 and +49 days.
                */
                newDay = number_of_days_in_month(startYear, static_cast<Month>(newMonth)); // Month changed, so does number of days in month.
            }
        }
        else
        {
            /*  Difference was positive, so adding = true. Now we are adding days (to Easter date in this assignment)
            */
            newDay++;
            if(newDay > number_of_days_in_month(startYear, static_cast<Month>(newMonth)))
            {
                // We went over that month`s days, so move to (month+1), and start from day 1
                newMonth ++ ;
                newDay = 1; // Month changed, so does we reset days to 1
            }
        }
    }
    std::cout << name << ": " << startYear << "/" << newMonth << "/" << newDay << std::endl;
}


void show_holy_days ()
{
    int inputYear   = 0,
        base        = 0;

    std::cout << "Input desired year:" << std::endl;
    std::cin >> inputYear;

    int easterMonth = easter_month(inputYear),
        easterDay   = easter_day(inputYear),
        holyMonth   = 0, // We'll use these to display holy days
        holyDay     = 0;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "---------------- HOLY CHRISTIAN DAYS " << inputYear << " ----------------" << std::endl;
    std::cout << "Easter: " << inputYear << "/" << easterMonth << "/" << easterDay << std::endl;

    /** Printing other holy days.
        Now, as we have easter month and day, we can calculate other holy days with known offsets.
    */

    /*  Carnival
        We know, that Carnival is 7 weeks before Easter, ending on Tuesday (we know that Easter is always on Sunday)
    */
    show_holy_day("Carnival", inputYear, easterMonth, easterDay, -47);

    /*  Good Friday
        This is Friday before Easter, so we have to subtract 2 days from Easter day.
    */
    show_holy_day("Good Friday", inputYear, easterMonth, easterDay, -2);


    /*  Ascension Day
        This is happening 10 days before Whitsuntide, which is 49 days after Easter. Hence it is on 39th day after Easter.
    */
    show_holy_day("Ascension Day", inputYear, easterMonth, easterDay, 39);

    /*  Whitsuntide
        This is happening 49 days after Easter.
    */
    show_holy_day("Whitsuntide", inputYear, easterMonth, easterDay, 49);

    // Ending print
    std::cout << "----------------------------------------------------------" << std::endl;
}

int main()
{
    char command = 'c';
    while(command == 'c' || command == 'C')
    {
        show_holy_days();

        std::cout << "Enter C to continue, anything else will exit the program:";
        std::cin >> command;
        std::cout << std::endl;
    }
    return 0;
}
