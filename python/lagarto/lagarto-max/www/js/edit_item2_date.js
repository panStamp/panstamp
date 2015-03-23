/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];

  fillDate(item2);
}

/**
 * Fill date fields
 */
function fillDate(item2)
{
  var fldMonth = document.getElementById("month");
	var fldDay = document.getElementById("day");

	for(i=1 ; i<=9 ; i++)
		fldDay.options[i-1] = new Option("0" + i, "0" + i);
	for(i=10 ; i<=31 ; i++)
		fldDay.options[i-1] = new Option(i, i);

  date = webToDate(item2);

  if (date != null)
  {
    if (date.length == 3)
    {
      fldMonth.value = date.substring(0, 1);
      fldDay.value = date.substring(1, 3);
    }
    else
    {
      fldMonth.value = date.substring(0, 2);
      fldDay.value = date.substring(2, 4);
    }
  }
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var month = document.getElementById("month").value;
  var day = document.getElementById("day").value;

  return parseInt(month) + day;
}

