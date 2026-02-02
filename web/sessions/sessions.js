
var option =
{
   animation: false,

   // sets the padding around the chart
   grid:
   {
      left: '1%',
      top: 10,
      right: '1%',
      bottom: 56
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
         type: "custom",

         encode: { x: [0,1], y: 0, },
         
         renderItem: (params, api) =>
         {              
            var timeSpan = [api.value(0), api.value(1)];
            var categoryIndex = api.value(2);

            var start = api.coord([timeSpan[0], categoryIndex]);
            var end   = api.coord([timeSpan[1], categoryIndex]);
            var size  = api.size([0,1]);

            // relative height of the bars  
            var height = size[1] * 0.8;

            // calculate width
            var width = end[0] - start[0];

            // enforce min width by adjusting x2
            var minWidth = 5;
            if (width < minWidth) end[0] = start[0] + minWidth;

            // text to show on element 
            var elementName = api.value(5); // player name
            if (api.value(2) == 0) elementName = `Level:${api.value(6)}`; // level number for type = gm

            // create rect and clip to chart area 
            var rect = echarts.graphic.clipRectByRect
            (
               { x: start[0], y: start[1] - height / 2, width: end[0] - start[0], height: height},
               { x: params.coordSys.x, y: params.coordSys.y, width: params.coordSys.width, height: params.coordSys.height},
            )
            if (rect !== undefined) rect.r = 4; // rounded corners

            var highlightBorderColor = getHSLC(api.value(3), 1.0, 180);
            if (api.value(2) == 0) highlightBorderColor = '#FF0000';

            const isSelected = api.value(8);            
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
                        text: elementName,
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
            var level    = params.data.value[6];
            var levelPad = String(level).padStart(3, '0');
            icon = `/assets/icons/lev${levelPad}.png`;

            var filename = params.data.value[7];
            var fullpath = "/downloads/" + filename;
            var link = `<a href="${fullpath}" download="${filename}">Download</a>`;

            name = '<div style="display: flex; gap: 10px">';
            name += '<img src="' + icon + '" style="width: 100px; height: 100px; vertical-align: middle;">';
            name += '<div style="display: flex; flex-direction:column; justify-content:space-evenly">';
            name += `Level:${level}` + link + '</div></div><hr>';
         }            

         // get date objects from start and end
         const d1 = new Date(params.data.value[0]);
         const d2 = new Date(params.data.value[1]);

         // get start, end and duration in string format
         const d1s = dateFns.format(d1, 'yyyy-MM-dd HH:mm:ss');
         const d2s = dateFns.format(d2, 'yyyy-MM-dd HH:mm:ss');
         const dur = getDuration(d1, d2);

         var s1 = '<span style="white-space:pre">';
         var eol = "</span><br>"; // end of line

         var txt =    name;
         txt +=    s1 + 'Start: '    + d1s  + eol;
         txt +=    s1 + 'End:   '    + d2s  + eol;
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
let categoryArray = [];

var currentSessionId;
var currentGmMuid;
var timelineHeight;

var timelineDataRangeOption;
var timelineViewRangeOption;

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

   timelineDataRangeOption = JSON.parse(sessionStorage.getItem("timelineDataRangeOption"));
   timelineViewRangeOption = JSON.parse(sessionStorage.getItem("timelineViewRangeOption"));


   // if any are not set (null), set default values
   if (currentSessionId  === null) fetchDataMostRecentSession();
   if (currentGmMuid     === null) fetchDataMostRecentGm();
   if (timelineHeight    === null) setTimelineHeight(400);
   if (colorSliderWidth  === null) colorSliderWidth = 60;   
   if (colorSliderEnable === null) colorSliderEnable = 0;

   if (timelineDataRangeOption === null) timelineDataRangeOption = 0;
   if (timelineViewRangeOption === null) timelineViewRangeOption = 0;
     

   // do these to save and initialize things associated with these variables
   setColorSliderWidth(colorSliderWidth);
   setColorSliderEnable(colorSliderEnable);


   
}


function checkLoaded()
{
   if ((currentGmMuid !== null) && (currentSessionId !== null)) 
   {
      // load initial
      fetchDataTimeline();
      fetchDataSessionsTable();
      fetchDataCurrentSession();
      fetchDataGmTable();
      fetchDataCurrentGm();
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
   const colorSliderWidthSlider = document.getElementById("colorSliderWidth");
   
   // set initial value
   colorSliderWidthSlider.value = colorSliderWidth;

   // listen for changes
   colorSliderWidthSlider.addEventListener("input", () =>
   {
      setColorSliderWidth(colorSliderWidthSlider.value);
   });


   // find color slider enable checkbox
   const colorSliderEnableCheckBox = document.getElementById("colorSliderEnable");
   
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
   document.documentElement.style.setProperty('--timelineHeight', `${val}px`);
}
function setColorSliderWidth(val)
{
   colorSliderWidth = val;
   sessionStorage.setItem("colorSliderWidth", JSON.stringify(colorSliderWidth));
   document.documentElement.style.setProperty('--colorSliderWidth', `${val}px`);
}

function setColorSliderEnable(val)
{
   colorSliderEnable = val;   
   sessionStorage.setItem("colorSliderEnable", JSON.stringify(colorSliderEnable));
   const myClassRule = getCSSRule('.colorSlider');
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


// iterate data and find range
// returns range array [0]-min [1]-max





function getDataRange()
{
   var min = 0;
   var max = 0;

   if (data.length)
   {
      // initial values are the first in the array
      var min = data[0].value[0];
      var max = data[0].value[1];
     
      // iterate the rest of the array
      for (let i = 1; i < data.length; i++)
      {
         if (data[i].value[0] < min) min = data[i].value[0];
         if (data[i].value[1] > max) max = data[i].value[1];
      }
   }
   document.getElementById("dataRangeStart" ).innerHTML = "Start: "    + getDateString(min);
   document.getElementById("dataRangeEnd"   ).innerHTML = "End: "      + getDateString(max);
   document.getElementById("dataDuration"   ).innerHTML = "Duration: " + getDuration(min, max);
   document.getElementById("dataItemCount"  ).innerHTML = "Items: "    + String(data.length);
   return [min, max];
}


// gets timeline chart actual view range
// iterates data and hides non visible elements
function getViewRange()
{
   var option = myChart.getOption();
   var min = option.dataZoom[0].startValue;
   var max = option.dataZoom[0].endValue;
   var view_item_count = 0;

   // hide data elements that are outside the range
   for (let i = 0; i < data.length; i++)
   {
      // if both start and end are before min or, both are after end, hide the element
      if (((data[i].value[0] < min) && (data[i].value[1] < min)) || ((data[i].value[0] > max) && (data[i].value[1] > max))) data[i].value[9] = 1;
      else
      {
         data[i].value[9] = 0;
         view_item_count++;
      }
   }
   document.getElementById("viewRangeStart" ).innerHTML = "Start: "    + getDateString(min);
   document.getElementById("viewRangeEnd"   ).innerHTML = "End: "      + getDateString(max);
   document.getElementById("viewDuration"   ).innerHTML = "Duration: " + getDuration(min, max);
   document.getElementById("viewItemCount"  ).innerHTML = "Items: "    + view_item_count;
}


function setViewRange()
{
   var minutes;

   switch (timelineViewRangeOption)
   {
      case 0: minutes = 0; break;   
      case 1: minutes = 10080; break;   
      case 2: minutes = 1440; break;   
      case 3: minutes = 720; break;   
      case 4: minutes = 60; break;   
   }

   if (minutes === 0)
   {
      myChart.dispatchAction({
         type: 'dataZoom',
         dataZoomIndex: 0, // Index of the dataZoom component to target
         start: 0,
         end: 100,
      });
   }
   else
   {
      var option = myChart.getOption();
      var d1 = option.dataZoom[0].startValue;
      var d2 = option.dataZoom[0].endValue;
      var newd2 = d1 + minutes * 60000;
      myChart.dispatchAction({
         type: 'dataZoom',
         dataZoomIndex: 0, // Index of the dataZoom component to target
         startValue: d1,
         endValue: newd2,
      });
   }
   getViewRange();
}

// iterate data and setup categories
function setCategories()
{
   // clear the category array
   categoryArray.length = 0;

   // iterate the data array
   for (let i = 0; i < data.length; i++)
   {
      // get name
      var name = data[i].value[4];

      // check if name already exists in categoryArray and get index if it does
      var categoryIndex = categoryArray.indexOf(name);
      
      // does not exist, create it and get index     
      if (categoryIndex === -1) categoryIndex = categoryArray.push(name) - 1;

      // set category index in data  
      data[i].value[2] = categoryIndex;
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
   if (ts === 0) return " ";
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
//   console.log("reloadChart()");
   var range = getDataRange();
   if (data.length == 0)
   {
      console.log("No data");
      categoryArray.length = 0;
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
      yAxis: { data: categoryArray },
      xAxis: { min: range[0], max: range[1] },
      series: [ { data: data } ]
   },
   false );

   setViewRange();
}


function setupTimelineDateRange()
{
   // put the datarange select here
   const container = document.getElementById('dataRangeSelectContainer');

/*   container.style.display = 'flex';
   container.style.gap = '12px';
   container.style.alignItems = 'center';
*/

   const selectArray = ['All', 'Last 30 days', 'Last 7 days', 'Last 24 hours', 'Last hour', 'Current session'];

   // label element
   const selectLabel = document.createElement('label');
   selectLabel.textContent = 'Data Range:';

   // select element
   const newSelect = document.createElement('select');
   newSelect.setAttribute('id', 'dataRangeSelect');
   newSelect.classList.add('select', 'timelineSelect');

   // fill select element with options from array
   var indx = 0; 
   for (const vs of selectArray)
   {
      const newOption = document.createElement('option');
      newOption.setAttribute('value', indx);
      newOption.textContent = vs;
      newSelect.appendChild(newOption);
      indx++;
   }


   // add label and select
   container.appendChild(selectLabel);
   selectLabel.appendChild(newSelect);

   // get element and set initial
   const selectElement = document.getElementById('dataRangeSelect');
   selectElement.selectedIndex = timelineDataRangeOption;
   // add listener
   selectElement.addEventListener("change", (event) =>
   { 
      timelineDataRangeOption = selectElement.selectedIndex;
      sessionStorage.setItem("timelineDataRangeOption", JSON.stringify(timelineDataRangeOption));
      fetchDataTimeline();
   });
   
}


function setupTimelineViewRange()
{
   // put the datarange select here
   const container = document.getElementById('viewRangeSelectContainer');

/*   container.style.display = 'flex';
   container.style.gap = '12px';
   container.style.alignItems = 'center';
*/

   const selectArray = ['Full', '7 days', '1 day', '12 hours', '1 hour'];

   // label element
   const selectLabel = document.createElement('label');
   selectLabel.textContent = 'View Range:';

   // select element
   const newSelect = document.createElement('select');
   newSelect.setAttribute('id', 'viewRangeSelect');
   newSelect.classList.add('select', 'timelineSelect');

   // fill select element with options from array
   var indx = 0; 
   for (const vs of selectArray)
   {
      const newOption = document.createElement('option');
      newOption.setAttribute('value', indx);
      newOption.textContent = vs;
      newSelect.appendChild(newOption);
      indx++;
   }

   // add label and select
   container.appendChild(selectLabel);
   selectLabel.appendChild(newSelect);

   // get element and set initial
   const selectElement = document.getElementById('viewRangeSelect');
   selectElement.selectedIndex = timelineViewRangeOption;

   // add listener
   selectElement.addEventListener("change", (event) =>
   { 
      timelineViewRangeOption = selectElement.selectedIndex;
      sessionStorage.setItem("timelineViewRangeOption", JSON.stringify(timelineViewRangeOption));
      setViewRange();
   });
}






























async function fetchDataTimeline()
{
   var timeline = new mtControls('timeline', fetchDataTimeline, 0, 0, 255, null);
   var tt = document.getElementById("timelineSectionContainer");
   var frame = 'divSectionContainer';
   if (timeline.minimized) frame = 'divSectionContainerMinimized';
   tt.innerHTML = `
   <div class="timeline ${frame} ">
      <div class="timeline divSectionTitleSectionFrame">
         <div class="timeline divSectionTitleSectionContainer">
            <div class="timeline divSectionTitleFrame">Sessions Timeline</div>
            <div class="timeline divSectionTitleFrameButtonsFrame">
               <div class="timeline-range-cont">
                  <table class="timelineRangeTable">
                     <tr> 
                        <td id="dataRangeSelectContainer"></td>
                        <td id="dataRangeStart"></td>
                        <td id="dataRangeEnd"></td>
                        <td id="dataDuration"></td>
                        <td id="dataItemCount"></td>
                     </tr> 
                     <tr> 
                        <td id="viewRangeSelectContainer"></td>
                        <td id="viewRangeStart"></td>
                        <td id="viewRangeEnd"></td>
                        <td id="viewDuration"></td>
                        <td id="viewItemCount"></td>
                     </tr> 
                  </table>
               </div>
               <label for="heightSlider" class="buttonLabel">Height:</label>
               <input style="width:60px" id="heightSlider" type="range" min=0 max=1200> 
               <div id="timelineControlsContainer"></div>
            </div>
         </div>
      </div>
      <div id="chartContainer"></div>
   </div>`;

   timeline.create();


   // set container height
   if (timeline.minimized) document.documentElement.style.setProperty('--timelineHeight', '0px');
   else document.documentElement.style.setProperty('--timelineHeight', `${timelineHeight}px`);

   // find height slider
   const heightSlider = document.getElementById("heightSlider");
   
   // set initial value
   heightSlider.value = timelineHeight;

   // listen for changes
   heightSlider.addEventListener("input", () =>
   {
      setTimelineHeight(heightSlider.value);
      if (timeline.minimized) document.documentElement.style.setProperty('--timelineHeight', '0px');
   });

   // find chart container
   const chartDom = document.getElementById('chartContainer');

   // create chart
   myChart = echarts.init(chartDom, 'dark',
   {
      renderer: 'svg',
      useDirtyRect: false,
      useCoarsePointer: true
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
      if (params.value[2]) // session
      {
         setCurrentSession(params.value[6]);
         fetchDataCurrentSession();
         fetchDataGmTable();
         if (timeline.view === 3) fetchData_timeline();
         else reloadChart();
      }
      else // gm
      {
         setCurrentGmMuid(params.value[5]);
         fetchDataCurrentGm();
         reloadChart();
      }
   });


   // listen for dataZoom changes
   myChart.on('datazoom', function (params)
   {
      getViewRange();
      myChart.setOption(
      {
         series: [ { data: data } ]
      },
      false );
   });

   setupTimelineDateRange();
   setupTimelineViewRange();


   if (data !== undefined) data.length = 0;
   var url = 'fetchDataTimeline.php';
   var range = timelineDataRangeOption;
   if (range == 0) url += '?range=all'; 
   if (range == 1) url += '?range=last&val=43200';
   if (range == 2) url += '?range=last&val=10080';
   if (range == 3) url += '?range=last&val=1440';
   if (range == 4) url += '?range=last&val=60';
   if (range == 5) url += '?range=session&val=' + currentSessionId;
   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }
      data = await response.json(); // Parse the response body as JSON
      reloadChart();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}







async function fetchDataSessionsTable()
{
   var sessionsTable = new mtControls('sessionsTable', fetchDataSessionsTable, 0, 0, 180, ['Default', 'Player', 'Bandwidth', 'All']);

   var url = 'fillSessionsTable.php?view=' + sessionsTable.view + '&min=' + sessionsTable.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("sessionsTableSectionContainer").innerHTML = html;

      // setup on click listeners
      const rows = document.querySelectorAll('#myTable tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('currentSessionId');
            if (id)
            {
               setCurrentSession(Number(id));
               fetchDataCurrentSession();
               fetchDataGmTable();
               fetchDataTimeline();
            }
         });
      });

      new DataTable("#myTable",
      {
         colReorder: true,
         columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: '_all' } ],
         order: [  [0, 'desc'] ]
      }  );
      sessionsTable.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataCurrentSession()
{
   var currentSession = new mtControls('currentSession', fetchDataCurrentSession, 0, 0, 120, ['Minimal', 'Player', 'Bandwidth']);
   var url = 'fillCurrentSession.php?id=' + currentSessionId + "&view=" + currentSession.view + '&min=' + currentSession.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("currentSessionSectionContainer").innerHTML = html;
      currentSession.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataGmTable()
{
   var gmTable = new mtControls('gmTable', fetchDataGmTable, 0, 0, 60, ['Current', 'All', 'Orphan']);

   var url = 'fillGmTable.php?id=' + currentSessionId + "&view=" + gmTable.view + '&min=' + gmTable.minimized;
   try
   {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();

      document.getElementById("gmTableSectionContainer").innerHTML = html;

      // setup on click listeners
      const rows = document.querySelectorAll('.gmTableTable tr');
      rows.forEach(row => {
         row.addEventListener('click', function() {
            const id = this.getAttribute('currentGmMuid');
            if (id)
            { 
               setCurrentGmMuid(id);
               fetchDataCurrentGm();
               reloadChart();
            }
         });
      });
      gmTable.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataCurrentGm()
{
   var currentGm = new mtControls('currentGm', fetchDataCurrentGm, 0, 1, 360, null );
   var url = 'fillCurrentGm.php?id=' + currentGmMuid + '&min=' + currentGm.minimized;
   try {
      const response = await fetch(url);
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      document.getElementById("currentGmSectionContainer").innerHTML = html;
      currentGm.create();
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataMostRecentSession()
{
   try {
      const response = await fetch('findMostRecentSession.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      setCurrentSession(Number(html));
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}

async function fetchDataMostRecentGm()
{
   try {
      const response = await fetch('findMostRecentGm.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      var html = await response.text();
      setCurrentGmMuid(html.trim());
   } catch (error) { console.error("Fetch error:", error.message); } // Handles network errors or the error thrown above
}






class mtControls
{
   constructor(n, udf, m, v, h, vsa)
   {
      this.name = n;
      this.updateFunction = udf;
      this.viewSelectArray = vsa;

      this.cssColorVariable = '--' + this.name + 'Color';
      this.containerId             = this.name + 'ControlsContainer';
      this.buttonId                = this.name + 'ButtonId';
      this.buttonClass             = this.name + 'ButtonClass';
      this.sliderId                = this.name + 'SliderId';
      this.selectId                = this.name + 'SelectId';

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
      const arrayString = JSON.stringify(myArray);
      sessionStorage.setItem(this.name, arrayString);
   }
   load()
   {
      const retrievedString = sessionStorage.getItem(this.name);
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
      // set the css color variable
      document.documentElement.style.setProperty(this.cssColorVariable, `hsl(${this.hue}, 100%, 50%)`);

      // create a <style> element for buttons and button:hover
      const style = document.createElement('style');
      style.type = 'text/css';
      style.innerHTML = `.${this.buttonClass}
      {
         --col: var(${this.cssColorVariable}); 
         color           : oklch(from var(--col) calc(l + 0.95) 0.1 h);
         border-color    : var(--col);
         background-color: oklch(from var(--col) 0.1 0.1 h);
      } `;

      style.innerHTML += `.${this.buttonClass}:hover  { border-color: oklch(from var(${this.cssColorVariable}) calc(l + 0.15) c h); }`;

      // append the style element to the document's <head>
      document.head.appendChild(style);

      // get the container and set flex
      const container = document.getElementById(this.containerId);
      container.style.display = 'flex';
      container.style.gap = '12px';
      container.style.alignItems = 'center';

      // create the view select, if there are any entries in viewSelectArray
      if ((this.viewSelectArray) && (this.viewSelectArray.length))
      {
         // label element
         const newSelectLabel = document.createElement('label');
         newSelectLabel.classList.add('buttonLabel', this.buttonClass);
         newSelectLabel.textContent = 'View:';

         // select element
         const newSelect = document.createElement('select');
         newSelect.setAttribute('id', this.selectId);
         newSelect.classList.add('select', this.buttonClass);

         // fill select element with options from array
         var indx = 0; 
         for (const vs of this.viewSelectArray)
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

         // get element and set initial
         var viewSelect = document.getElementById(this.selectId);
         viewSelect.selectedIndex = this.view;
         // add listener

         viewSelect.addEventListener("change", (event) =>
         { 
            this.setView(viewSelect.selectedIndex);
            this.updateFunction(); 
         });
      }


      // create the color hue slider
      const newInput = document.createElement('input');
      newInput.setAttribute('id', this.sliderId);
      newInput.classList.add('colorSlider');
      newInput.setAttribute('type', 'range');
      newInput.setAttribute('min', '0');
      newInput.setAttribute('max', '360');

      container.appendChild(newInput);

      const hueSlider = document.getElementById(this.sliderId);
      hueSlider.value = this.hue;
      hueSlider.addEventListener("input", () =>
      {
         this.setHue(hueSlider.value);
         document.documentElement.style.setProperty(this.cssColorVariable, `hsl(${this.hue}, 100%, 50%)`);
         //console.log(hueSlider.value);
      });


      // create the minimize button
      const newButton = document.createElement('button');

      newButton.textContent = this.minimized ? '+' : '-';
      newButton.setAttribute('id', this.buttonId);
      newButton.classList.add("minimizeButton", this.buttonClass);

      container.appendChild(newButton);

      document.getElementById(this.buttonId).addEventListener("click", () =>
      {
         this.toggleMinimized();
         this.updateFunction();
      });
   }
}

