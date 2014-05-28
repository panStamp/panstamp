/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.condition[3];
  document.getElementById("wday").value = weekDayToInt(item2);
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  return document.getElementById("wday").value;
}

