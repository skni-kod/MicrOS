#include "RTC.h"

#define CURRENT_YEAR 2018 // Change this each year!

int century_register = 0x00; // Set by ACPI table parsing code if possible

enum
{
      cmos_address = 0x70,
      cmos_data = 0x71
};

int get_update_in_progress_flag()
{
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg)
{
      uint8_t NMI_bit = inb(cmos_data) & 0x80;
      outb(NMI_bit | cmos_address, reg);
      return inb(cmos_data);
}

void read_rtc(rtc_time *time)
{
      unsigned char century;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      time->second = get_RTC_register(0x00);
      time->minute = get_RTC_register(0x02);
      time->hour = get_RTC_register(0x04);
      time->day = get_RTC_register(0x07);
      time->month = get_RTC_register(0x08);
      time->year = get_RTC_register(0x09);
      if (century_register != 0)
      {
            century = get_RTC_register(century_register);
      }

      do
      {
            last_second = time->second;
            last_minute = time->minute;
            last_hour = time->hour;
            last_day = time->day;
            last_month = time->month;
            last_year = time->year;
            last_century = century;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            time->second = get_RTC_register(0x00);
            time->minute = get_RTC_register(0x02);
            time->hour = get_RTC_register(0x04);
            time->day = get_RTC_register(0x07);
            time->month = get_RTC_register(0x08);
            time->year = get_RTC_register(0x09);
            if (century_register != 0)
            {
                  century = get_RTC_register(century_register);
            }
      } while ((last_second != time->second) || (last_minute != time->minute) || (last_hour != time->hour) ||
               (last_day != time->day) || (last_month != time->month) || (last_year != time->year) ||
               (last_century != century));

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary

      if (!(registerB & 0x04))
      {
            time->second = (time->second & 0x0F) + ((time->second / 16) * 10);
            time->minute = (time->minute & 0x0F) + ((time->minute / 16) * 10);
            time->hour = ((time->hour & 0x0F) + (((time->hour & 0x70) / 16) * 10)) | (time->hour & 0x80);
            time->day = (time->day & 0x0F) + ((time->day / 16) * 10);
            time->month = (time->month & 0x0F) + ((time->month / 16) * 10);
            time->year = (time->year & 0x0F) + ((time->year / 16) * 10);
            if (century_register != 0)
            {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      // Convert 12 hour clock to 24 hour clock if necessary

      if (!(registerB & 0x02) && (time->hour & 0x80))
      {
            time->hour = ((time->hour & 0x7F) + 12) % 24;
      }

      // Calculate the full (4-digit) year

      if (century_register != 0)
      {
            time->year += century * 100;
      }
      else
      {
            time->year += (CURRENT_YEAR / 100) * 100;
            if (time->year < CURRENT_YEAR)
                  time->year += 100;
      }
}

uint8_t read_second()
{
      unsigned char last_second;
      unsigned char second;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      second = get_RTC_register(0x00);

      do
      {
            last_second = second;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            second = get_RTC_register(0x00);

      } while (last_second != second);

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary

      if (!(registerB & 0x04))
      {
            second = (second & 0x0F) + ((second / 16) * 10);
      }

      return second;
}

uint8_t read_minute()
{
      unsigned char last_minute;
      unsigned char minute;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      minute = get_RTC_register(0x02);

      do
      {
            last_minute = minute;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            minute = get_RTC_register(0x02);

      } while (last_minute != minute);

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary

      if (!(registerB & 0x04))
      {
            minute = (minute & 0x0F) + ((minute / 16) * 10);
      }

      return minute;
}

uint8_t read_hour()
{
      unsigned char last_hour;
      unsigned char hour;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      hour = get_RTC_register(0x04);

      do
      {
            last_hour = hour;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            hour = get_RTC_register(0x04);

      } while (last_hour != hour);

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary
      if (!(registerB & 0x04))
      {
            hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
      }

      if (!(registerB & 0x02) && (hour & 0x80))
      {
            hour = ((hour & 0x7F) + 12) % 24;
      }

      return hour;
}

uint8_t read_day()
{
      unsigned char last_day;
      unsigned char day;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      day = get_RTC_register(0x07);

      do
      {
            last_day = day;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            day = get_RTC_register(0x07);

      } while (last_day != day);

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary
      if (!(registerB & 0x04))
      {
            day = (day & 0x0F) + ((day / 16) * 10);
      }

      return day;
}

uint8_t read_month()
{
      unsigned char last_month;
      unsigned char month;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      month = get_RTC_register(0x08);

      do
      {
            last_month = month;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            month = get_RTC_register(0x08);

      } while (last_month != month);

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary
      if (!(registerB & 0x04))
      {
            month = (month & 0x0F) + ((month / 16) * 10);
      }

      return month;
}

uint32_t read_year()
{
      unsigned char century;
      unsigned char last_century;
      unsigned char last_year;
      uint32_t year;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag())
            ; // Make sure an update isn't in progress
      year = get_RTC_register(0x09);

      if (century_register != 0)
      {
            century = get_RTC_register(century_register);
      }

      do
      {
            last_year = year;
            last_century = century;

            while (get_update_in_progress_flag())
                  ; // Make sure an update isn't in progress
            year = get_RTC_register(0x09);
            if (century_register != 0)
            {
                  century = get_RTC_register(century_register);
            }

      } while ((last_year != year) || (last_century != century));

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary
      if (!(registerB & 0x04))
      {
            year = (year & 0x0F) + ((year / 16) * 10);
            if (century_register != 0)
            {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      // Calculate the full (4-digit) year
      if (century_register != 0)
      {
            year += century * 100;
      }
      else
      {
            year += (CURRENT_YEAR / 100) * 100;
            if (year < CURRENT_YEAR)
                  year += 100;
      }

      return year;
}
