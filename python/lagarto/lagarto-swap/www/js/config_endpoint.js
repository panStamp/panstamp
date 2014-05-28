/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var endpoint_id = getUrlVars()["id"];

  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.network;

  swapnet.motes.forEach(function(mote)
  {
    mote.registers.forEach(function(register)
    {
      register.endpoints.forEach(function(endpoint)
      {
        if (endpoint.id == endpoint_id)
        {
          document.getElementById("motename").value = mote.name;
          document.getElementById("developer").value = mote.manufacturer;
          document.getElementById("moteaddr").value = mote.address;

          document.getElementById("regname").value = register.name;
          document.getElementById("regid").value = register.id;

          document.getElementById("location").value = endpoint.location;
          document.getElementById("name").value = endpoint.name;
          document.getElementById("id").value = endpoint.id;
          document.getElementById("type").value = endpoint.type;
          document.getElementById("direction").value = endpoint.direction;
          document.getElementById("value").value = endpoint.value;

          if (endpoint.timestamp != null)
            document.getElementById("timestamp").value = endpoint.timestamp;

          if (endpoint.unit != null)
          {
            var opt;
            var unitField = document.getElementById("unit");
            endpoint.units.forEach(function(unit)
            {
              opt = document.createElement('option');
              opt.text = unit;
              opt.value = unit;
              unitField.add(opt);
            });
            unitField.value = endpoint.unit;
          }
          loadJSONdata("../values/?id=" + getUrlVars()["id"], updateEndp);
          return;
        }
      });
    });
  });
}

/**
 * Update endpoint data
 */
function updateEndp()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  //for each (var endpoint in swapnet.status)
  swapnet.status.forEach(function(endpoint)
  {
    document.getElementById("location").value = endpoint.location;
    document.getElementById("name").value = endpoint.name;
    document.getElementById("value").value = endpoint.value;

    if (endpoint.unit != null)
      document.getElementById("unit").value = endpoint.unit;

    if (endpoint.timestamp != null)
      document.getElementById("timestamp").value = endpoint.timestamp;
  });
}

