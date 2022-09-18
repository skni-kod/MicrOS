#include "rtc.h"

#define CURRENT_YEAR 2018 // Change this each year!

int century_register = 0x32; // Set by ACPI table parsing code if possible

// Register  Contents
//  0x00      Seconds
//  0x02      Minutes
//  0x04      Hours
//  0x06      Weekday
//  0x07      Day of Month
//  0x08      Month
//  0x09      Year
//  0x32      Century (maybe)
//  0x0A      Status Register A
//  0x0B      Status Register B

enum
{
      cmos_address = 0x70,
      cmos_data = 0x71
};

int get_update_in_progress_flag()
{
      io_out_byte(cmos_address, 0x0A);
      return (io_in_byte(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg)
{
      uint8_t NMI_bit = io_in_byte(cmos_address) & 0x80;
      io_out_byte(cmos_address, reg | NMI_bit);
      return io_in_byte(cmos_data);
}

void rtc_read(rtc_time *time)
{
      unsigned char century = 0;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t rtc_read_second()
{
      unsigned char last_second;
      unsigned char second;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t rtc_read_minute()
{
      unsigned char last_minute;
      unsigned char minute;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t rtc_read_hour()
{
      unsigned char last_hour;
      unsigned char hour;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t rtc_read_day()
{
      unsigned char last_day;
      unsigned char day;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t rtc_read_month()
{
      unsigned char last_month;
      unsigned char month;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint32_t rtc_read_year()
{
      unsigned char century = 0;
      unsigned char last_century;
      unsigned char last_year;
      uint32_t year;
      unsigned char registerB;

      // This uses the "read registers until you get the same values twice in a row" technique
      // to avoid getting dodgy/inconsistent values due to RTC updates

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

uint8_t BINtoBCD(uint8_t n)
{
      uint8_t j = n % 10;
      uint8_t d = n / 10;

      uint8_t value = j + (d << 4);
      return value;
}

void rtc_set(rtc_time *time)
{
      //Disable interrupts
      io_disable_interrupts();

      uint8_t registerB = get_RTC_register(0x0B);
      //change values to BCD
      if (!(registerB & 0x04))
      {
            time->second = BINtoBCD(time->second);
            time->minute = BINtoBCD(time->minute);
            time->hour = BINtoBCD(time->hour);
            time->day = BINtoBCD(time->day);
            time->month = BINtoBCD(time->month);
            time->year = BINtoBCD(time->year % 100);
      }

      //Setting 0x80 byte to disable NMI

      io_out_byte(0x70, 0x80);
      io_out_byte(0x71, time->second);

      io_out_byte(0x70, 0x82);
      io_out_byte(0x71, time->minute);

      io_out_byte(0x70, 0x84);
      io_out_byte(0x71, time->hour);

      io_out_byte(0x70, 0x87);
      io_out_byte(0x71, time->day);

      io_out_byte(0x70, 0x88);
      io_out_byte(0x71, time->month);

      io_out_byte(0x70, 0x89);
      io_out_byte(0x71, time->year);

      //enable interrupts
      io_enable_interrupts();

      //enable NMI
      io_out_byte(0x70, io_in_byte(0x70) & 0x7F);
}

void rtc_enable_IRQ8()
{
      io_disable_interrupts();        // disable interrupts
      io_out_byte(0x70, 0x8B);        // select register B, and disable NMI
      char prev = io_in_byte(0x71);   // read the current value of register B
      io_out_byte(0x70, 0x8B);        // set the index again (a read will reset the index to register D)
      io_out_byte(0x71, prev | 0x40); // write the previous value ORed with 0x40. This turns on bit 6 of register B
      io_enable_interrupts();
      io_out_byte(0x70, io_in_byte(0x70) & 0x7F); // enable NMI
}

void rtc_disable_IRQ8()
{
      io_disable_interrupts();        // disable interrupts
      io_out_byte(0x70, 0x8B);        // select register B, and disable NMI
      char prev = io_in_byte(0x71);   // read the current value of register B
      io_out_byte(0x70, 0x8B);        // set the index again (a read will reset the index to register D)
      io_out_byte(0x71, prev & 0xBF); // write the previous value ORed with 0xBF. This turns off bit 6 of register B
      io_enable_interrupts();
      io_out_byte(0x70, io_in_byte(0x70) & 0x7F); // enable NMI
}

void rtc_change_interrupt_rate(uint8_t rate)
{
      rate &= 0x0F; // rate must be above 2 and not over 15 to get proper result
      io_disable_interrupts();
      io_out_byte(0x70, 0x8A);                 // set index to register A, disable NMI
      char prev = io_in_byte(0x71);            // get initial value of register A
      io_out_byte(0x70, 0x8A);                 // reset index to A
      io_out_byte(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.
      io_enable_interrupts();
      io_out_byte(0x70, io_in_byte(0x70) & 0x7F); // enable NMI
}