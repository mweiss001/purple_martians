
var option =
{

   toolbox:
   {
      show: true,
      feature:
      {
          dataZoom: { show: true, yAxisIndex: 'none'  },
      }
   },


   xAxis:
   {
      type: "time",

      axisLabel:
      {
         formatter: function (value)
         {
           return echarts.format.formatTime('yyyyMMdd\nhh:mm:ss', value);
         },
        showMinLabel: true,
        showMaxLabel: true,
        interval: 'auto' 
       },


      // vertical line at mouse position
      axisPointer:
      {
         show: true,
         type: "line",
         lineStyle:
         {
//            color: "#FFFFFF",
         },


         // label at bottom of line, just above x axis       
         label:
         {
            show: 'true',
//            color: "#FFFFFF",
//            fontSize: 12,
            backgroundColor: "#111111",
            borderColor: "#DDDDDD",
            borderWidth: 1,
            
         },
      },
   },

   yAxis: { type: "category", },


   series:
   [
      {
         type: "custom",
         encode: { x: [0,1], y: 0, },
         renderItem: (params, api) =>
         {              
            const isSelected = api.value(8);

//            console.log(isSelected); 

            var timeSpan = [api.value(0), api.value(1)];
            var catIndex = api.value(2);

            var start = api.coord([timeSpan[0], catIndex]);
            var end   = api.coord([timeSpan[1], catIndex]);
            var size  = api.size([0,1]);

            // relative height of the bars  
            var height = size[1] * 0.8;

            // calculate width
            var width = end[0] - start[0];

            // enforce min width by adjusting x2
            var minWidth = 5;
            if (width < minWidth) end[0] = start[0] + minWidth;

            var name1 = api.value(6); // player name
            if (api.value(2) == 0) name1 = `Level:${api.value(6)}`; // level number for type = session



                 
            // actual timespan in seconds 
//            var ts = (api.value(1) - api.value(0)) / 1000;
//                 console.log("catIndex:%d - seconds:%d  width: ", catIndex, ts, width);
//                 console.log("start   :", start);
//                 console.log("end     :", end);
//                 console.log("width   :", width);

            // create rect and clip to charts area 
            var rect = echarts.graphic.clipRectByRect
            (
               { x: start[0], y: start[1] - height / 2, width: end[0] - start[0], height: height},
               { x: params.coordSys.x, y: params.coordSys.y, width: params.coordSys.width, height: params.coordSys.height},
            )
            if (rect !== undefined) rect.r = 8; // rounded corners

            return {




               type: 'group',
               children:
               [
                  {
                     type: 'rect',
                     transition: ["shape"],
                     shape: rect,
                     style:
                     {

                        fill: isSelected ?




                        {
                           type: 'linear',
                           x: 0, y: 0, x2: 0, y2: 1,
                           colorStops:
                           [
                              { offset: 0.0, color: getHSL(api.value(3), 1.0) },
                              { offset: 0.5, color: getHSLC(api.value(3), 1.0, 180) },
                              { offset: 1.0, color: getHSL(api.value(3), 1.0) }


                          ]
                        }



                         :




                        {
                           type: 'linear',
                           x: 0, y: 0, x2: 0, y2: 1,
                           colorStops:
                           [
                              { offset: 0.0, color: getHSL(api.value(3), 0.2) },
                              { offset: 1.0, color: getHSL(api.value(3), 1.0) }
                          ]
                        },
                        








          
//                       fill: isSelected ? 'red' : 'blue' // Manual color toggle
/*

                        fill:
                        {
                           type: 'linear',
                           x: 0, y: 0, x2: 0, y2: 1,
                           colorStops:
                           [
                              { offset: 0.0, color: getHSL(api.value(3), 0.2) },
                              { offset: 1.0, color: getHSL(api.value(3), 1.0) }
                          ]
                        }

*/


                     },

/*                                      
                     select:  // Highlight style when selected
                     {         
                        style:
                        {
//                           fill: '#be123c', // Example: Red color
                           focus: 'self', 

                           stroke: '#0000ff',
                           lineWidth: 4,

                           //borderColor: '#FF0000',        
                           //borderWidth: 4, 
                           //focus: 'self', 


                           //shadowBlur: 10,
                           //shadowColor: 'rgba(0, 0, 0, 0.5)'


                        }
                     },



                     emphasis: // Optional: Hover style
                     { 
                        style:
                        {
//                           fill: '#4338ca', // Example: Blue color
                           stroke: '#ffffff',
                           lineWidth: 2,

                           focus: 'self', 
                        }
                     },

*/
/*                     

    emphasis: {
        focus: 'series', // Highlights the current series and blurs others
        itemStyle: {
            shadowBlur: 10,
            shadowColor: 'rgba(0, 0, 0, 0.5)'
        }
    },

*/
                     
                     
                     
                     
                     
                     
                     
                     
                  },
                  {
                     type: 'text',
                     style:
                     {
                          text: name1,
                          x: start[0]+8, 
                          y: (start[1]-height/2) + 8,
                          fill: '#FFFFFF',
                          textAlign: 'left',
                          textVerticalAlign: 'top',
                          width: width, 
                          overflow: 'truncate', 

                      }
                  }
               ]
            };
         },
      }
   ],



   tooltip:
   {
      //confine: true,

      //position: 'inside',
      position: 'top',
      //position: 'bottom',

      hideDelay: 500,

      backgroundColor: 'rgba(50, 50, 50, 1.0)',
      //backgroundColor: 'rgba(50, 50, 50, 0.9)',      
      borderColor: '#aaaaaa',
      borderWidth: 1,

      borderRadius: 8,

//  tb, lr
//      padding: [4, 8], 
// t r b l
      padding: [4, 8, 2, 8],



      enterable: true,

      extraCssText: 'pointer-events: auto|important', 

      textStyle:
      {
         color: '#fff',
         fontFamily: 'monospace',
         //fontSize: 16,
      },

      show: true,
      trigger: "item",

      formatter: params =>
      {
         var name = "";
         var icon = "";
         
         if (params.data.value[2]) // type = session
         {
            icon = `/assets/icons/player_icon_${params.data.value[3]}.png`;
            name = '<img src="' + icon + '" style="width: 20px; height: 20px; vertical-align: middle;">';
            name += '<span style="white-space:pre"> ';
            name += params.data.value[4]; // player name
            name += '</span><hr>';
         }
         else // type = gm
         {
            var lev = params.data.value[6];
            var l2 = String(lev).padStart(3, '0');
            icon = `/assets/icons/lev${l2}.png`;

            var filename = params.data.value[7];
            var fullpath = "/downloads/" + filename;
            var lnk = `<a href="${fullpath}" download="${filename}">Download</a>`;

            name = '<div style="display: flex; gap: 10px">';
            name += '<img src="' + icon + '" style="width: 100px; height: 100px; vertical-align: middle;">';
            name += '<div style="display: flex; flex-direction:column; justify-content:space-evenly">';
            name += `Level:${lev}` + lnk + '</div></div><hr>';
         }            

         // get date objects from start and end
         const d1 = new Date(params.data.value[0]);
         const d2 = new Date(params.data.value[1]);

         // format start and end
         const fst = dateFns.format(d1, 'yyyy-MM-dd HH:mm:ss');
         const fet = dateFns.format(d2, 'yyyy-MM-dd HH:mm:ss');

         // get duration
         const dur = getDuration(d1, d2);

         var s1 = '<span style="white-space:pre">';
         var eol = "</span><br>"; // end of line

         var txt =    name;
         txt +=    s1 + 'Start: '    + fst  + eol;
         txt +=    s1 + 'End:   '    + fet  + eol;
         txt +=    s1 + 'Duration: ' + dur  + eol;

         return txt;
      },
   },

   dataZoom:
   [
      {
         type: "slider",
         filterMode: "none"
      },
   ],
}


var myChart;
var sessionsTableView;
var gmTableView;
var timelineRange;
var currentSessionId;
var currentSessionView;
var currentGmMuid;
var data;
var maxCat;
let catArr = [];


/*
function resetHighlight()
{
   console.log("resetHighlight()");
   myChart.setOption( { series: [ { data: data } ]}, false);
}
const intervalId = setInterval(resetHighlight, 1000);
*/



function readVariablesFromSessionStorage()
{
   // attempt to read all variables from session storage
   currentSessionId   = JSON.parse(sessionStorage.getItem("currentSessionId"));
   currentSessionView = JSON.parse(sessionStorage.getItem("currentSessionView"));
   sessionsTableView  = JSON.parse(sessionStorage.getItem("sessionsTableView"));
   gmTableView        = JSON.parse(sessionStorage.getItem("gmTableView"));
   currentGmMuid      = JSON.parse(sessionStorage.getItem("currentGmMuid"));
   timelineRange      = JSON.parse(sessionStorage.getItem("timelineRange"));
 
   // if any are not set (null), set default values
   if (currentSessionId === null)
   {
       console.log("currentSessionId not set...finding most recent");
       fetchDataMostRecentSession();
   }
   if (currentGmMuid === null)
   {
       console.log("currentGmMuid not set...finding most recent");
       fetchDataMostRecentGm();
   }
   if (currentSessionView === null)
   {
      currentSessionView = 1;
      sessionStorage.setItem("currentSessionView", JSON.stringify(currentSessionView));
   }
   if (sessionsTableView === null)
   {
      sessionsTableView = 1;
      sessionStorage.setItem("sessionsTableView", JSON.stringify(sessionsTableView));
   }

   if (gmTableView === null)
   {
      gmTableView = 1;
      sessionStorage.setItem("gmTableView", JSON.stringify(gmTableView));
   }
   if (timelineRange === null)
   {
      timelineRange = 0;
      sessionStorage.setItem("timelineRange", JSON.stringify(timelineRange));
   }





}


// -----------------------------------------------
// ---   setup once the page has loaded   ---
// -----------------------------------------------
document.addEventListener('DOMContentLoaded', (event) =>
{
   readVariablesFromSessionStorage();

   myChart = echarts.init(document.getElementById('chart-container'), 'dark',
   {
      renderer: 'canvas',
      useDirtyRect: false
   });

   // initial options load for chart
   if (option && typeof option === 'object') myChart.setOption(option);

   // listen for timeline chart clicks
   myChart.on('click', function (params)
   {
      // iterate the array and set current selection
      for (let i = 0; i < data.length; i++)
      {
         data[i].value[8] = 0; // unset
         if (data[i].value[2] && (data[i].value[6] === params.value[6])) data[i].value[8] = 1;
      }

      if (params.value[2]) setCurrentSession(params.value[6]); // session
      else                 setCurrentGmMuid(params.value[5]);  // gm
   });

   // listen for window resize events 
   window.addEventListener('resize', myChart.resize);




document.getElementById("testButton1").addEventListener("click", function()
{
   console.log("test button 1 pressed");   
});
document.getElementById("testButton2").addEventListener("click", function()
{
   console.log("test button 2 pressed");   
});


   // range selection drop down list
   var rangeSelect = document.getElementById("range-select");
   // set initial
   rangeSelect.selectedIndex = timelineRange;
   // add listener
   rangeSelect.addEventListener("change", (event) =>
   {
      timelineRange = rangeSelect.selectedIndex;
      sessionStorage.setItem("timelineRange", JSON.stringify(timelineRange));
      fetchDataTimeline();
   });


   // load initial
   fetchDataTimeline();
   fetchDataCurrentSession();
   fetchDataSessionsTable();
   fetchDataGmTable();
   fetchDataCurrentGm();
   
   
});

function setCurrentSession(id)
{
   currentSessionId = id;
   sessionStorage.setItem("currentSessionId", JSON.stringify(currentSessionId));
   fetchDataCurrentSession();
   fetchDataGmTable();


   fetchDataTimeline();
/*
   // reload timeline if range is set to current session 
   if (timelineRange === 3) fetchDataTimeline();

   else
   {
      // iterate the array and set current selection
      for (let i = 0; i < data.length; i++)
      {
         data[i].value[8] = 0; // unset
         if (data[i].value[2] && (data[i].value[6] === currentSessionId)) data[i].value[8] = 1;
      }
      myChart.setOption( { series: [ { data: data } ]}, false);
   }

*/



}


function setCurrentGmMuid(id)
{
   currentGmMuid = id;
   sessionStorage.setItem("currentGmMuid", JSON.stringify(currentGmMuid));
   fetchDataCurrentGm();
}



const playerColors = [
   "#888888", 
   "#bf6ce8", 
   "#8820ac",
   "#3c7fff",
   "#e01c48",
   "#ff00e8",
   "#ffbf7f",
   "#ff7f00",
   "#7f00ff",
   "#01ff7f",
   "#ff0000",
   "#01ff00",
   "#0000ff",
   "#01ffff",
   "#ffff00",
   "#ffffff",
]

const playerColorsHSL = [
  [   0,    0,   53 ],
  [ 280,   73,   67 ],
  [ 285,   69,   40 ],
  [ 219,  100,   62 ],
  [ 347,   78,   49 ],
  [ 305,  100,   50 ],
  [  30,  100,   75 ],
  [  30,  100,   50 ],
  [ 270,  100,   50 ],
  [ 150,  100,   50 ],
  [   0,  100,   50 ],
  [ 120,  100,   50 ],
  [ 240,  100,   50 ],
  [ 180,  100,   50 ],
  [  60,  100,   50 ],
  [   0,    0,  100 ]
]

function getHSL(p, lm)
{
   var h = playerColorsHSL[p][0];
   var s = playerColorsHSL[p][1];
   var l = playerColorsHSL[p][2] * lm;
   var ret = "hsl(" + h + ", " + s + "%, " + l + "%)";
   return ret;
}



function getHSLC(p, lm, cs)
{
   var h = playerColorsHSL[p][0];

   h+=cs;
   if (h>360) h-=360;

   var s = playerColorsHSL[p][1];
   var l = playerColorsHSL[p][2] * lm;
   var ret = "hsl(" + h + ", " + s + "%, " + l + "%)";
   return ret;
}







function getDuration(d1, d2)
{
   const duration = dateFns.intervalToDuration({ start: d1, end: d2 });
   var returnText = "";
   const zeroPad = (num) => String(num).padStart(2, '0');

   const parts = [
      duration.days,
      duration.hours,
      duration.minutes,
      duration.seconds
   ];

   if (duration.hours   === undefined) duration.hours   = 0;
   if (duration.minutes === undefined) duration.minutes = 0;
   if (duration.seconds === undefined) duration.seconds = 0;
    
   if (duration.days !== undefined)
   {
      // this will only be the remaining days after accounting for full years and months, get actual days here
      returnText = dateFns.differenceInDays(d2, d1) + " ";
   }
  returnText += zeroPad(duration.hours) + ":" + zeroPad(duration.minutes) + ":" + zeroPad(duration.seconds);

   // pad to 16 char
   returnText = returnText.padStart(16);

   return returnText;
}





// iterate data and finds min start and max end
// returns range array [0]-min [1]-max
function getDataRange()
{
   if (data.length == 0)
   {
      document.getElementById("num_items").innerHTML   = "Items: 0";
      document.getElementById("range-start").innerHTML = "Start: ";
      document.getElementById("range-end"  ).innerHTML = "End: ";
      return [0, 0];
   }
    

   // initial values are the first in the array
   var min = data[0].value[0];
   var max = data[0].value[1];
  
   // iterate the array
   for (let i = 1; i < data.length; i++)
   {
      if (data[i].value[0] < min) min = data[i].value[0];
      if (data[i].value[1] > max) max = data[i].value[1];
   }

   // get date objects fron start and end
   const d1 = new Date(min);
   const d2 = new Date(max);

   // format start and end
   const fst = dateFns.format(d1, 'yyyy-MM-dd HH:mm:ss');
   const fet = dateFns.format(d2, 'yyyy-MM-dd HH:mm:ss');
   
   document.getElementById("num_items").innerHTML   = "Items: " + String(data.length);
   document.getElementById("range-start").innerHTML = "Start: " + fst;
   document.getElementById("range-end"  ).innerHTML = "End: "   + fet;

   return [min, max];
}


// iterate data and setup categories
function setCategories()
{
   // clear the category array
   catArr.length = 0;

   // iterate the data array
   for (let i = 0; i < data.length; i++)
   {
      // get name
      var name = data[i].value[4];

      // check if name already exists in catArr and get index if it does
      var catIndex = catArr.indexOf(name);
      
      // does not exist, create it and get index     
      if (catIndex === -1) catIndex = catArr.push(name) - 1;

      // set category index in data  
      data[i].value[2] = catIndex;
   }

   maxCat = catArr.length-1; 
}


function setChartCurrentSelection()
{
   // iterate the array and set current selection
   for (let i = 0; i < data.length; i++)
   {
      data[i].value[8] = 0; // unset
      if (data[i].value[2] && (data[i].value[6] === currentSessionId)) data[i].value[8] = 1;
   }
}


function reloadChart()
{
   var range = getDataRange();

   if (data.length == 0)
   {
      console.log("No data");
      catArr.length = 0;
      maxCat = 0;
   }
   else
   {
      // modify the array, add key-value 'name' and name from value 4
      data = data.map(obj => { return { name: obj.value[4], ...obj  }; });
      setCategories();
      setChartCurrentSelection();
   }

   myChart.setOption(
   {
      yAxis: { data: catArr },
      xAxis: { min: range[0], max: range[1] },
      series: [ { data: data } ]
   }, false);
}




async function fetchDataTimeline()
{
   if (data !== undefined) data.length = 0;
   var url = 'fetch_data_timeline.php';
   if (timelineRange == 0) url += '?range=all'; 
   if (timelineRange == 1) url += '?range=last&val=60';
   if (timelineRange == 2) url += '?range=last&val=1440';
   if (timelineRange == 3) url += '?range=session&val=' + currentSessionId;
   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }
      data = await response.json(); // Parse the response body as JSON
      reloadChart();
   } catch (error)
   {
      console.error("Fetch error:", error.message); // Handles network errors or the error thrown above
   }
}



async function fetchDataSessionsTable()
{
   var url = 'fill_sessions_table.php?view=' + sessionsTableView;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("sessions_table_js_target").innerHTML = html;

      // session table view selection drop down list
      var sessionsTableViewSelect = document.getElementById("sessions_view_select");
      // set initial
      sessionsTableViewSelect.selectedIndex = sessionsTableView;
      // add listener
      sessionsTableViewSelect.addEventListener("change", (event) =>
      { 
         sessionsTableView = sessionsTableViewSelect.selectedIndex;
         sessionStorage.setItem("sessionsTableView", JSON.stringify(sessionsTableView));
         fetchDataSessionsTable();
      });


      // setup on click listeners
      const rows = document.querySelectorAll('#myTable tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('current_session_id');
            if (id) setCurrentSession(Number(id));
         });
      });

      new DataTable("#myTable",
      {
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: '_all' } ],
         order: [  [0, 'desc'] ]
      }  );


   } catch (error)
   {
      console.error("Fetch error:", error.message); // Handles network errors or the error thrown above
   }
}

async function fetchDataCurrentSession()
{
   var url = 'fill_current_session.php?id=' + currentSessionId + "&view=" + currentSessionView;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("current_session_js_target").innerHTML = html;

   // current session view selection drop down list
   var currentSessionViewSelect = document.getElementById("current_session_view_select");
   // set initial
   currentSessionViewSelect.selectedIndex = currentSessionView;
   // add listener
   currentSessionViewSelect.addEventListener("change", (event) =>
   { 
      currentSessionView = currentSessionViewSelect.selectedIndex;
      sessionStorage.setItem("currentSessionView", JSON.stringify(currentSessionView));
      fetchDataCurrentSession();
   });



   } catch (error)
   {
      console.error("Fetch error:", error.message); // Handles network errors or the error thrown above
   }
}




















async function fetchDataGmTable()
{
   var url = 'fill_gm_table.php?id=' + currentSessionId + "&view=" + gmTableView;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();

      document.getElementById("gm_table_js_target").innerHTML = html;

      // setup on click listeners
      const rows = document.querySelectorAll('#gm_table_body tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('current_gm_muid');
            if (id) setCurrentGmMuid(id);
         });
      });
   // gm table view selection drop down list
   var gmTableViewSelect = document.getElementById("gm_table_view_select");
   // set initial
   gmTableViewSelect.selectedIndex = gmTableView;
   // add listener
   gmTableViewSelect.addEventListener("change", (event) =>
   { 
      gmTableView = gmTableViewSelect.selectedIndex;
      sessionStorage.setItem("gmTableView", JSON.stringify(gmTableView));
      fetchDataGmTable();
   });
   } catch (error)
   {
      console.error("Fetch error:", error.message); // Handles network errors or the error thrown above
   }
}

async function fetchDataCurrentGm()
{
   var url = 'fill_current_gm.php?id=' + currentGmMuid;
   try {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("current_gm_js_target").innerHTML = html;
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataMostRecentSession()
{
   try {
      const response = await fetch('find_most_recent_session.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      setCurrentSession(Number(html));
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataMostRecentGm()
{
   try {
      const response = await fetch('find_most_recent_gm.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      setCurrentGmMuid(html.trim());
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}
































