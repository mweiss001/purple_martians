



var option =
{
   animation: false,


   // sets the padding around the chart
   grid:
   {
//      left: 72,
      left: '1%',
      top: 10,
      right: '1%',
      bottom: 48
   },

/*
   // the only ones I use are the zoom and restore zoom
   toolbox:
   {
      show: true,
      feature:
      {
          dataZoom: { show: true, yAxisIndex: 'none'  },











      }
   },

*/

   xAxis:
   {
      type: "time",

      axisLabel: { show: false },

/*
      
      // ticks with labels 
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
*/








      // vertical line at mouse position
      axisPointer:
      {
         show: true,
         type: "line",
         // lineStyle: { color: "#FFFFFF" },

         // label at bottom of line, in the middle of zoom bar
         label:
         {
            show: 'true',
            margin: 17, 
            padding: [3, 4, 2, 4], // Custom padding
            //color: "#FFFFFF",
            //fontSize: 12,
            backgroundColor: "#111111",
            borderColor: "#DDDDDD",
            borderWidth: 1,
         },
      },
 },



   yAxis:
   {
      type: "category",
      axisLabel: { show: false }
   },


   series:
   [
      {





         roam: true,

         type: "custom",
         encode: { x: [0,1], y: 0, },
         renderItem: (params, api) =>
         {              
            const isSelected = api.value(8);

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


            // text to show on element 


//            var name1 = api.value(6); // session id
//            var name1 = api.value(7); // host name
//            var name1 = `${api.value(5)} - ${api.value(9)}`; // player name

            var name1 = api.value(5); // player name

            if (api.value(2) == 0) name1 = `Level:${api.value(6)}`; // level number for type = gm



                 
            // actual timespan in seconds 
//            var ts = (api.value(1) - api.value(0)) / 1000;
//                 console.log("catIndex:%d - seconds:%d  width: ", catIndex, ts, width);
//                 console.log("start   :", start);
//                 console.log("end     :", end);
//                 console.log("width   :", width);



            // create rect and clip to chart area 
            var rect = echarts.graphic.clipRectByRect
            (
               { x: start[0], y: start[1] - height / 2, width: end[0] - start[0], height: height},
               { x: params.coordSys.x, y: params.coordSys.y, width: params.coordSys.width, height: params.coordSys.height},
            )
            if (rect !== undefined) rect.r = 4; // rounded corners

            var highlightBorderColor = getHSLC(api.value(3), 1.0, 180);
            if (api.value(2) == 0) highlightBorderColor = '#FF0000';

                        
            if (api.value(9)) return null; // hidden 
            else return {
               type: 'group',
               children:
               [
                  // main gradient filled rect
                  {
                     type: 'rect',
                     transition: ["shape"],
                     shape: rect,
                     style:
                     {
                        fill:
                        {
                           type: 'linear',
                           x: 0, y: 0, x2: 0, y2: 1,
                           colorStops:
                           [
                              { offset: 0.0, color: getHSL(api.value(3), 0.1) },
                              { offset: 1.0, color: getHSL(api.value(3), 1.0) }
                           ]
                        },
                     },
                  },
                   
                  // highlight selected items with rect frame
                  {
                     type: 'rect',
                     shape: rect,
                     style:
                     {
                        fill: 'none', 
                        lineWidth: 2,  
                        stroke: isSelected ? highlightBorderColor : 'none',
                     }
                  },

                  // text overlay 
                  {
                     type: 'text',
                     style:
                     {
                        text: name1,
                        fill: '#FFFFFF',
                        x: start[0]+8, 
                        y: (start[1]-height/2) + 8,
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
      position: 'top',
      backgroundColor: 'rgba(50, 50, 50, 1.0)',
      borderColor: '#aaaaaa',
      borderWidth: 1,
      borderRadius: 8,
      padding: [4, 8, 2, 8],

      hideDelay: 500,
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
         type: 'slider',
         filterMode: "none",
      },
      {
         type: 'inside',
         filterMode: "none",
      },
   ],
}

var myChart;
var data;
let catArr = [];

var currentSessionId;
var currentGmMuid;
var timelineHeight;

let colorSliderWidth = 60;
let colorSliderEnable = 0;



function readVariablesFromSessionStorage()
{
   // attempt to read all variables from session storage
   currentSessionId   = JSON.parse(sessionStorage.getItem("currentSessionId"));
   currentGmMuid      = JSON.parse(sessionStorage.getItem("currentGmMuid"));
   timelineHeight     = JSON.parse(sessionStorage.getItem("timelineHeight"));
   colorSliderWidth   = JSON.parse(sessionStorage.getItem("colorSliderWidth"));
   colorSliderEnable  = JSON.parse(sessionStorage.getItem("colorSliderEnable"));


   // if any are not set (null), set default values
   if (currentSessionId  === null) fetchDataMostRecentSession();
   if (currentGmMuid     === null) fetchDataMostRecentGm();
   if (timelineHeight    === null) setTimelineHeight(400);
   if (colorSliderWidth  === null) colorSliderWidth = 60;   
   if (colorSliderEnable === null) colorSliderEnable = 0;
      

   // do these to save and initialize things associated with these variables
   setColorSliderWidth(colorSliderWidth);
   setColorSliderEnable(colorSliderEnable);


   
}


function checkLoaded()
{
   if ((currentGmMuid !== null) && (currentSessionId !== null)) 
   {
      // load initial
      fetchData_timeline();
      fetchData_sessions_table();
      fetchData_current_session();
      fetchData_gm_table();
      fetchData_current_gm();
   } 
   else
   {
      console.log('Variables not set yet, checking again...');
      setTimeout(checkLoaded, 100); // Check again in 100ms
   }
}


function getCSSRule(ruleName)
{
   ruleName = ruleName.toLowerCase();
   for (const styleSheet of document.styleSheets)
   {
      for (const cssRule of styleSheet.cssRules)
      {
         if (cssRule.selectorText && cssRule.selectorText.toLowerCase() === ruleName)
         {
           return cssRule;
         }
      }
   }
   return null;
}

/*
// Usage:
const myClassRule = getCSSRule('.myClass');
if (myClassRule) {
  myClassRule.style.backgroundColor = 'red';
}
*/



// -----------------------------------------------
// ---   setup once the page has loaded   ---
// -----------------------------------------------
document.addEventListener('DOMContentLoaded', (event) =>
{
   readVariablesFromSessionStorage();
   checkLoaded();

   const dialog = document.getElementById("settingsDialog");
   const openButton = document.getElementById("openSettings");

   // Open the dialog
   openButton.addEventListener("click", () => {
     dialog.showModal(); // Use showModal() for modal behavior
   });

   // find color slider width slider
   const colorSliderWidthSlider = document.getElementById("color-slider-width");
   
   // set initial value
   colorSliderWidthSlider.value = colorSliderWidth;

   // listen for changes
   colorSliderWidthSlider.addEventListener("input", () =>
   {
      setColorSliderWidth(colorSliderWidthSlider.value);
   });


   // find color slider enable checkbox
   const colorSliderEnableCheckBox = document.getElementById("color-slider-enable");
   
   // set initial value
   colorSliderEnableCheckBox.checked = colorSliderEnable;

   // listen for changes
   colorSliderEnableCheckBox.addEventListener("input", () =>
   {
      if (colorSliderEnableCheckBox.checked) colorSliderEnable = 1;
      else colorSliderEnable = 0;

      setColorSliderEnable(colorSliderEnable);


   });







/*
   document.getElementById("testButton1").addEventListener("click", function()
   {
      console.log("test button 1 pressed");   
   });
   document.getElementById("testButton2").addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
   });

*/

});



function setCurrentSession(id)
{
   currentSessionId = id;
   sessionStorage.setItem("currentSessionId", JSON.stringify(currentSessionId));
}
function setCurrentGmMuid(id)
{
   currentGmMuid = id;
   sessionStorage.setItem("currentGmMuid", JSON.stringify(currentGmMuid));
}
function setTimelineHeight(val)
{
   timelineHeight = val;
   sessionStorage.setItem("timelineHeight", JSON.stringify(timelineHeight));
   document.documentElement.style.setProperty('--timeline-height', `${val}px`);
}
function setColorSliderWidth(val)
{
   colorSliderWidth = val;
   sessionStorage.setItem("colorSliderWidth", JSON.stringify(colorSliderWidth));
   document.documentElement.style.setProperty('--color_slider-width', `${val}px`);
}

function setColorSliderEnable(val)
{
   colorSliderEnable = val;   
   sessionStorage.setItem("colorSliderEnable", JSON.stringify(colorSliderEnable));
   const myClassRule = getCSSRule('.color_slider');
   if (myClassRule)
   {
      if (colorSliderEnable) myClassRule.style.display = 'block';
      else                   myClassRule.style.display = 'none';
   }
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
   
   document.getElementById("num_items").innerHTML   = "Items: " + String(data.length);
   document.getElementById("range-start").innerHTML = "Start: " + getDateString(min);
   document.getElementById("range-end"  ).innerHTML = "End: "   + getDateString(max);

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
}


function setChartCurrentSelection()
{
   // iterate the array and set current selection
   for (let i = 0; i < data.length; i++)
   {
      data[i].value[8] = 0; // unset
      if (data[i].value[2] && (data[i].value[6] === currentSessionId)) data[i].value[8] = 1;
      if (!data[i].value[2] && (data[i].value[5] === currentGmMuid)) data[i].value[8] = 1;
   }
}

function getDateString(ts)
{
   const d = new Date(ts);
   const f = dateFns.format(d, 'yyyy-MM-dd HH:mm:ss');
   return f;
}


function logData()
{
   var test = [];
   for (let i = 0; i < data.length; i++) test.push(`${data[i].value[0]} - ${data[i].value[5]}`);
   console.log(test);
}

function sortData()
{
   data.sort((a, b) => a.value[0] - b.value[0]);
//   logData();
}


function reloadChart()
{
   //console.log("reloadChart()");

   var range = getDataRange();
   if (data.length == 0)
   {
      console.log("No data");
      catArr.length = 0;
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
   },
   false );
}




async function fetchData_timeline()
{

   var timeline = new mtControls('timeline', fetchData_timeline, 0, 0, 255);
   timeline.view_select_array = ['All', 'Last Hour', 'Last Day', 'Current Session'];

   var tt = document.getElementById("timeline_js_target");
  
   tt.innerHTML = `
   <div class="timeline div-section-container">
      <div class="timeline div-section-title-section-frame">
         <div class="timeline div-section-title-section-container">
            <div class="timeline div-section-title-frame">Sessions Timeline</div>
            <div class="timeline div-section-title-frame-buttons-frame">
               <button class="button timeline_button" id="vrange-start"></button>
               <button class="button timeline_button" id="vrange-end"></button>
               <button class="button timeline_button" id="num_items"></button>
               <button class="button timeline_button" id="range-start"></button>
               <button class="button timeline_button" id="range-end"></button>
               <input class="button timeline_button"  id="height-slider" type="range" min=0 max=800> 
               <div id="timeline-cont"></div>
            </div>
         </div>
      </div>
      <div id="chart-container"></div>
   </div>
`;

   timeline.create();

   // set container height
   if (timeline.minimized) document.documentElement.style.setProperty('--timeline-height', '0px');
   else document.documentElement.style.setProperty('--timeline-height', `${timelineHeight}px`);

   // find height slider
   const heightSlider = document.getElementById("height-slider");
   
   // set initial value
   heightSlider.value = timelineHeight;

   // listen for changes
   heightSlider.addEventListener("input", () =>
   {
      setTimelineHeight(heightSlider.value);
      if (timeline.minimized) document.documentElement.style.setProperty('--timeline-height', '0px');
   });


   // find chart container
   const chartDom = document.getElementById('chart-container');

   // create chart
   myChart = echarts.init(chartDom, 'dark',
   {
      renderer: 'canvas',
      useDirtyRect: false
   });

   // create observer to detect container size changes
   const resizeObserver = new ResizeObserver(() => { myChart.resize(); });
   resizeObserver.observe(chartDom);

   // initial options load for chart
   if (option && typeof option === 'object') myChart.setOption(option);

   // listen for timeline chart clicks
   myChart.on('click', function (params)
   {
      //console.log("timeline click");
      
//      setChartCurrentSelection();

      if (params.value[2]) // session
      {
         setCurrentSession(params.value[6]);
         fetchData_current_session();
         fetchData_gm_table();
         if (timeline.view === 3) fetchData_timeline();
         else reloadChart();
      }
      else // gm
      {
         setCurrentGmMuid(params.value[5]);
         fetchData_current_gm();
         reloadChart();
      }
   });


   // listen for dataZoom changes
   myChart.on('datazoom', function (params)
   {
      var option = myChart.getOption();

      // get view range
      var min = option.dataZoom[0].startValue;
      var max = option.dataZoom[0].endValue;

      document.getElementById("vrange-start").innerHTML = "Start: " + getDateString(min);
      document.getElementById("vrange-end"  ).innerHTML = "End: "   + getDateString(max);

      // hide data elements that are outside the range
      for (let i = 0; i < data.length; i++)
      {
         // if both start and end are before min or, both are after end, hide the element
         if (((data[i].value[0] < min) && (data[i].value[1] < min)) || ((data[i].value[0] > max) && (data[i].value[1] > max))) data[i].value[9] = 1;
         else data[i].value[9] = 0;
      }
      reloadChart();  
   });


   if (data !== undefined) data.length = 0;
   var url = 'fetch_data_timeline.php';
   var range = timeline.view;
   if (range == 0) url += '?range=all'; 
   if (range == 1) url += '?range=last&val=60';
   if (range == 2) url += '?range=last&val=1440';
   if (range == 3) url += '?range=session&val=' + currentSessionId;
   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }

/*
      data = await response.text(); // Parse the response body as JSON
      console.log(data);
      data = data.json();
*/

      data = await response.json(); // Parse the response body as JSON

      reloadChart();

   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}



async function fetchData_sessions_table()
{
   var sessions_table = new mtControls('sessions_table', fetchData_sessions_table,  0, 0, 180);
   sessions_table.view_select_array = ['Default', 'Player', 'Bandwidth', 'All'];

   var url = 'fill_sessions_table.php?view=' + sessions_table.view + '&min=' + sessions_table.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("sessions_table_js_target").innerHTML = html;

      // setup on click listeners
      const rows = document.querySelectorAll('#myTable tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('current_session_id');
            if (id)
            {
               setCurrentSession(Number(id));
               fetchData_current_session();
               fetchData_gm_table();
               fetchData_timeline();
            }
         });
      });

      new DataTable("#myTable",
      {
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: '_all' } ],
         order: [  [0, 'desc'] ]
      }  );
      sessions_table.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchData_current_session()
{
   var current_session = new mtControls('current_session', fetchData_current_session, 0, 0, 120);
   current_session.view_select_array = ['Minimal', 'Player', 'Bandwidth'];
   var url = 'fill_current_session.php?id=' + currentSessionId + "&view=" + current_session.view + '&min=' + current_session.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("current_session_js_target").innerHTML = html;
      current_session.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchData_gm_table()
{
   var gm_table = new mtControls('gm_table', fetchData_gm_table,  0, 0, 60);
   gm_table.view_select_array = ['Current', 'All', 'Orphan'];
   var url = 'fill_gm_table.php?id=' + currentSessionId + "&view=" + gm_table.view + '&min=' + gm_table.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();

      document.getElementById("gm_table_js_target").innerHTML = html;

      // setup on click listeners
      const rows = document.querySelectorAll('.gm_tablet tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('current_gm_muid');
            if (id)
            { 
               setCurrentGmMuid(id);
               fetchData_current_gm();
               reloadChart();
            }
         });
      });
      gm_table.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchData_current_gm()
{
   var current_gm = new mtControls('current_gm', fetchData_current_gm, 0, 1, 360);
   var url = 'fill_current_gm.php?id=' + currentGmMuid + '&min=' + current_gm.minimized;
   try {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("current_gm_js_target").innerHTML = html;
      current_gm.create();
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
































class mtControls
{
   name;
   update_function;
   css_color_var;
   class_button;
   cont_id;
   min_button_id;
   hue_slider_id;
   view_select_id;
   view_select_array;
   minimized;
   view;
   hue;

   constructor(n, udf, m, v, h)
   {
      this.name = n;

      this.update_function = udf;

      this.css_color_var = '--' + this.name + '-color';

      this.cont_id        = this.name    + '-cont';
      this.min_button_id  = this.cont_id + '-min';
      this.hue_slider_id  = this.cont_id + '-hue';
      this.view_select_id = this.cont_id + '-view';

      this.class_button = this.name + '_button';

      this.minimized = m;
      this.view = v;
      this.hue = h;

      this.load();

   }

   setHue(h)
   {
      this.hue = Number(h);
      this.save();
   }

   toggleMinimized()
   {
      this.minimized = Number(!this.minimized);
      this.save();
   }

   setMinimized(m)
   {
      this.minimized = Number(m);
      this.save();
   }

   setView(v)
   {
      this.view = Number(v);
      this.save();
   }

   save()
   {
      const myArray = [this.minimized, this.view, this.hue];

      // Convert the array to a JSON string
      const arrayString = JSON.stringify(myArray);

      // Store the string in sessionStorage
      sessionStorage.setItem(this.name, arrayString);
   }
   load()
   {
      // Retrieve the string from sessionStorage
      const retrievedString = sessionStorage.getItem(this.name);

      // Convert the string back into a JavaScript array
      const retrievedArray = JSON.parse(retrievedString);

      if (retrievedArray)
      {
         this.minimized = Number(retrievedArray[0]);
         this.view      = Number(retrievedArray[1]);
         this.hue       = Number(retrievedArray[2]);
      }
      else this.save();
   }

   create()
   {
      //console.log("class mtControls.create()", this.name);

      // set the css color variable
      document.documentElement.style.setProperty(this.css_color_var, `hsl(${this.hue}, 100%, 50%)`);


      // create a <style> element for buttons and button:hover
      const style = document.createElement('style');
      style.type = 'text/css';

      style.innerHTML = `.${this.class_button}
      {
         --col: var(${this.css_color_var}); 
         color           : oklch(from var(--col) calc(l + 0.95) 0.1 h);
         border-color    : var(--col);
         background-color: oklch(from var(--col) 0.1 0.1 h);
      } `;

      style.innerHTML += `.${this.class_button}:hover  { border-color: oklch(from var(${this.css_color_var}) calc(l + 0.15) c h); }`;

      // append the style element to the document's <head>
      document.head.appendChild(style);


      // get the container and set flex
      const container = document.getElementById(this.cont_id);
      container.style.display = 'flex';
      container.style.gap = '12px';
      container.style.alignItems = 'center';
      //container.innerHTML = '';


      // create the view select
      // if there are any entries in view
      if (this.view_select_array)
      {
         // lebel element
         const newSelectLabel = document.createElement('label');
         newSelectLabel.classList.add('button-label', this.class_button);
         newSelectLabel.textContent = 'View:';

         // select element
         const newSelect = document.createElement('select');
         newSelect.setAttribute('id', this.view_select_id);
         newSelect.classList.add('button', this.class_button);

         // fill select element with options from array
         var indx = 0; 
         for (const vs of this.view_select_array)
         {
            const newOption = document.createElement('option');
            newOption.setAttribute('value', indx);
            newOption.textContent = vs;
            newSelect.appendChild(newOption);
            indx++;
         }

         // add label and select
         container.appendChild(newSelectLabel);
         newSelectLabel.appendChild(newSelect);
//         container.appendChild(newSelect);

         // get element and set initial
         var viewSelect = document.getElementById(this.view_select_id);
         viewSelect.selectedIndex = this.view;
         // add listener
         viewSelect.addEventListener("change", (event) =>
         { 
            this.setView(viewSelect.selectedIndex);
            this.update_function();         
         });
      }


      // create the color hue slider
      const newInput = document.createElement('input');
      newInput.setAttribute('id', this.hue_slider_id);
      newInput.classList.add('color_slider');
      newInput.setAttribute('type', 'range');
      newInput.setAttribute('min', '0');
      newInput.setAttribute('max', '360');







      container.appendChild(newInput);

      const hueSlider = document.getElementById(this.hue_slider_id);
      hueSlider.value = this.hue;
      hueSlider.addEventListener("input", () =>
      {
         this.setHue(hueSlider.value);
         document.documentElement.style.setProperty(this.css_color_var, `hsl(${this.hue}, 100%, 50%)`);
         //console.log(hueSlider.value);
      });


      // create the minimize button
      const newButton = document.createElement('button');
      



      newButton.textContent = this.minimized ? '+' : '-';
      newButton.setAttribute('id', this.min_button_id);
      newButton.classList.add("min-button", this.class_button);

      container.appendChild(newButton);

      document.getElementById(this.min_button_id).addEventListener("click", () =>
      {
         this.toggleMinimized();
         this.update_function();
      });
   }
}

