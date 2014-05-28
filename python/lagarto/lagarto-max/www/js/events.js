/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var eventId;
var months = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
var wdays = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"];

/**
 * Convert [1, 12] integer into month string
 */
function intToMonth(val)
{
  if (val > 0 && val < 13)
    return months[val-1];
  return null;
}

/**
 * Convert month string into [1, 12] integer
 */
function monthToInt(month)
{
  var i;

  for(i=0 ; i<months.length ; i++)
  {
    if (months[i] == month)
      return i+1;
  }
  return null;
}

/**
 * Convert [0, 6] integer into week day string
 */
function intToWeekDay(val)
{
  if (val >= 0 && val < 7)
    return wdays[val];
  return null;
}

/**
 * Convert week day string into [0, 6] integer
 */
function weekDayToInt(wday)
{
  var i;

  for(i=0 ; i<wdays.length ; i++)
  {
    if (wdays[i] == wday)
      return i;
  }
  return null;
}

/**
 * Convert "mmdd" date into web representation
 */
function dateToWeb(date)
{
  var intMonth, intDay;

  if (date.length == 3)
  {
    intMonth = parseInt(date.substring(0, 1));
    intDay = parseInt(date.substring(1, 3));
  }
  else
  {
    intMonth = parseInt(date.substring(0, 2));
    intDay = parseInt(date.substring(2, 4));
  }

  return intToMonth(intMonth) + ", " + intDay;
}

/**
 * Convert web representation into "mmdd" date
 */
function webToDate(strDate)
{
  var start = 0, end, month, day;

  end = strDate.indexOf(",");
  if (end > -1)
  {
    month = monthToInt(strDate.substring(start, end));

    start = end + 2;
    day = strDate.substring(start);
    if (day.length == 1)
      day = "0" + day;
    
    return month + day;
  }

  return null;
}

/**
 * Convert "hhmm" time into web representation
 */
function timeToWeb(time)
{
  var ldif = 4 - time.length;
  for(var i=0 ; i<ldif ; i++)
    time = "0" + time;

  return time.substring(0, 2) + ":" + time.substring(2, 4);
}

/**
 * Convert web representation into "hhmm" time
 */
function webToTime(strTime)
{
  if (strTime.indexOf(":") == 2)
  {
    return parseInt(strTime.substring(0, 2) + strTime.substring(3, 5));
  }

  return null;
}

/**
 * Return Web representation of a given condition
 */
function conditionToWeb(line)
{
  var type = "network", item1 = "", operator = "", item2 = "";
  var start, end;

  // Item1
  if (line.substring(0, 6) == "clock.")
  {
    type = "clock";

    end = line.indexOf("repeat_time(", 6);
    if (end > -1)
    {
      var initTime, endTime;

      item1 = "time";
      operator = "==";

      start = end + 12;
      end = line.indexOf(", ", start);

      if (end > -1)
      {
        initTime = line.substring(start, end);
        start = end + 2;
        end = line.indexOf(")", start);
        endTime = line.substring(start, end);
        item2 = timeToWeb(initTime) + " repeat every " + timeToWeb(endTime);

        return [type, item1, operator, item2];
      }
      return null;
    }
    else
    {
      end = line.indexOf("()", 6);
      if (end > -1)
        item1 = line.substring(6, end);
    }
  }
  else if (line.substring(0, 19) == "network.get_value(\"")
  {
    end = line.indexOf("\")", 19);
    if (end > -1)
      item1 =  line.substring(19, end);
  }
  else if (line.substring(0, 21) == "network.event[0] == \"")
  {
    var endShift = 22;

    start = 21;
    end = line.indexOf("\" and network.event[1]", start);
    if (end == -1)
    {
      end = line.indexOf("\" and float(network.event[1])", start);
      endShift = 29;
    }
    if (end > -1)
    {
      item1 =  line.substring(start, end);
      line = line.substring(end + endShift);
    }
    else
    {
      end = line.indexOf('"', start);
      if (end > -1)
      {
        item1 =  line.substring(start, end);
        return [type, item1, "on change", ""];
      }
    }
  }

  // Operator
  start = line.indexOf(" ");
  if (start > -1)
  {
    end = line.indexOf(" ", start+1);
    if (end > -1)
    {
      operator = line.substring(start+1, end);

      // Item2
      start = end + 1;
      item2 = line.substring(start);

      if (type == "network")
      {
        start = item2.indexOf("network.get_value(\"");
        if (start > -1)
        {
          start += 19;
          end = item2.indexOf("\")", start);
          if (end > -1)
            item2 = item2.substring(start, end);
        }
      }
      else if (item1 == "time")
      {
        if (item2.substring(0, 6) == "clock.")
        {
          start = 6;
          end = item2.indexOf("(", start);
          var func = item2.substring(start, end);

          if (func == "repeat_time")
            item2 = timeToWeb(item2.substring(18, 22)) + " repeat every " + timeToWeb(item2.substring(24, 28));
          else
            item2 = func
        }
        else
          item2 = timeToWeb(item2);
      }
      else if (item1 == "weekday")
        item2 = intToWeekDay(item2);
      else if (item1 == "month")
        item2 = intToMonth(item2);
      else if (item1 == "date" && item1.length == 4)
        item2 = dateToWeb(item2);
      else
        item2 = item2.replace(/\"/g, "");
    }
  }

  return [type, item1, operator, item2];
}

/**
 * Get Web representation of a given action
 */
function actionToWeb(line)
{
  var type = "network", item1 = "", operator = "=", item2 = "";
  var start, end;

  if (line.substring(0, 19) == "network.set_value(\"")
  {
    end = line.indexOf("\", ", 19);
    if (end > -1)
    {
      item1 =  line.substring(19, end);
      start = end + 3
      end = line.indexOf(")");
      if (end > -1)
      {
        item2 = line.substring(start, end);
        item2 = item2.replace(/\"/g, "");
      }
    }
  }
  else if (line.substring(0, 11) == "cloud.push_")
  {
    type = "cloud";
    operator = "update";
    start = 11;
    end = line.indexOf("(\"", start);
    if (end > -1)
    {
      item1 =  line.substring(start, end);
      start = end + 2
      end = line.indexOf("\"", start);
      if (end > -1)
        item2 = line.substring(start, end);
    }
  }

  return [type, item1, operator, item2];
}

