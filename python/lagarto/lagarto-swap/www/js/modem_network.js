/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var xmlDoc = getXmlDoc();
  var section = xmlDoc.getElementsByTagName("network")[0];

  if (section != null)
  {
    var opt;

    var channel = section.getElementsByTagName("channel")[0];
    if (channel != null)
    {
      var field = document.getElementById("channel");
      for(i=0 ; i<10 ; i++)
      {
        opt = document.createElement('option');
        opt.text = "channel " + i;
        opt.value = i;
        field.add(opt);
      }
      field.value = channel.childNodes[0].nodeValue;
    }

    var netid = section.getElementsByTagName("netid")[0];
    if (netid != null)
    document.getElementById("netid").value = netid.childNodes[0].nodeValue;

    var address = section.getElementsByTagName("address")[0];
    if (address != null)
    {
      var field = document.getElementById("address");
      for(i=1 ; i<=255 ; i++)
      {
        opt = document.createElement('option');
        opt.text = i;
        opt.value = i;
        field.add(opt);
      }
      field.value = address.childNodes[0].nodeValue;
    }

    var security = section.getElementsByTagName("security")[0];
    if (security != null)
    document.getElementById("security").value = security.childNodes[0].nodeValue;

    var password = section.getElementsByTagName("password")[0];
    if (password != null)
    document.getElementById("password").value = password.childNodes[0].nodeValue;
  }
}

