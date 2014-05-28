/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];

  changeOption();

  fillTime(item2);
}

/**
 * Enable/disable repeat option
 */
function changeOption()
{
  if (window.parent.statementType == "trigger")
  {
	  if (window.parent.statement[2] == "==")
		  document.getElementById("item2option").style.visibility="visible";
	  else
    {
		  document.getElementById("item2option").style.visibility="hidden";
      document.getElementById("freqhour").value == "00";
      document.getElementById("freqmin").value == "00";
    }
  }
}

/**
 * Fill time fields
 */
function fillTime(item2)
{
	var fldHour = document.getElementById("hours");
	var fldFreqHour = document.getElementById("freqhour");
	var fldMin = document.getElementById("minutes");
	var fldFreqMin = document.getElementById("freqmin");  
	var index1, index2, hour = 0, minute = 0;

  // Hours
	for(i=0;i<24;i++)
	{
		fldHour.options[i] = new Option("0" + i, i);
		fldFreqHour.options[i] = new Option("0" + i, i);
	}
	for(i=10;i<24;i++)
	{
		fldHour.options[i] = new Option(i, i);
		fldFreqHour.options[i] = new Option(i, i);
	}
  // Minutes
	for(i=0;i<=9;i++)
	{
		fldMin.options[i] = new Option("0" + i, i);
		fldFreqMin.options[i] = new Option("0" + i, i);
	}
	for(i=10;i<60;i++)
	{
		fldMin.options[i] = new Option(i, i);
		fldFreqMin.options[i] = new Option(i, i);
	}

	if ((index1 = item2.indexOf(":", 0)) != -1)
	{
    hour = parseInt(item2.substring(0, index1));
		index1++;
		minute = parseInt(item2.substring(index1, index1+2));
	}
  fldHour.value = hour;
  fldMin.value = minute;

  hour = 0;
  minute = 0;
	if ((index1 = item2.indexOf(" repeat every ", index1+2)) != -1)
  {
		index1 += 14;
		if ((index2 = item2.indexOf(":", index1)) != -1)
    {
			hour = parseInt(item2.substring(index1, index2));
      minute = parseInt(item2.substring(index2+1, index2+3));
    }
	}
	fldFreqHour.value = hour;
	fldFreqMin.value = minute;
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var pythonString = parseInt(document.getElementById("hours").value) * 100 + parseInt(document.getElementById("minutes").value);
  var interval = parseInt(document.getElementById("freqhour").value) * 100 + parseInt(document.getElementById("freqmin").value);

  if (window.parent.statement[2] == "==" && interval > 0)
    pythonString = "clock.repeat_time(" + pythonString + ", " + interval + ")";

  return pythonString;
}

/**
 * Switch to sunlight time
 */
function switchItem2()
{
	window.parent.document.getElementById("item2box").src = "../edit_item2_sunl.html";
}

