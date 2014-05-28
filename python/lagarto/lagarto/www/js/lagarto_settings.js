/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var xmlDoc = getXmlDoc();
  var section = xmlDoc.getElementsByTagName("lagarto")[0];

  if (section != null)
  {
    var opt;

    var procname = section.getElementsByTagName("procname")[0];
    if (procname != null)
      document.getElementById("procname").value = procname.childNodes[0].nodeValue;

    var comms = xmlDoc.getElementsByTagName("comms")[0];
    if (comms != null)
    {
      var broadcast = comms.getElementsByTagName("broadcast")[0];
      if (broadcast != null)
        document.getElementById("broadcast").value = broadcast.childNodes[0].nodeValue;

      var httpport = comms.getElementsByTagName("httpport")[0];
      if (httpport != null)
        document.getElementById("httpport").value = httpport.childNodes[0].nodeValue;
    }
  }
}

