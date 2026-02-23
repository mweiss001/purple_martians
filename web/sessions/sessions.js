
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

                     // text overlay 
                     textContent:
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
                           height: height,
                           overflow: 'truncate', 
                        },
                        textConfig: { position: 'inside' } // Position text inside rect
                     }
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
               ]
            };
         },
         clip: true,
      }
   ],

   tooltip:
   {
//      position: 'top',


        position: function (point, params, dom, rect, size) {
            // point is the current mouse position (e.g., [x, y]) in pixel coordinates

            // Example 1: Offset the tooltip by a fixed amount (10px right, 10px down)
            // return [point[0] + 10, point[1] + 10];

            // Example 2: Place the tooltip at a specific Y-coordinate (e.g., 20% from the top)
            // return [point[0], '20%'];

            // Example 3: More advanced positioning to ensure it stays within the chart boundaries
            var x = point[0];
            var y = point[1];
            var tooltipWidth = size.contentSize[0];
            var tooltipHeight = size.contentSize[1];
            var chartWidth = size.viewSize[0];

            // If the tooltip would go off the right edge, flip it to the left
            if (x + tooltipWidth > chartWidth) {
                x -= tooltipWidth + 20; // add some padding
            } else {
                x += 10; // default offset
            }
            
            // Similar check for vertical position
            if (y + tooltipHeight > size.viewSize[1]) {
                y -= tooltipHeight;
            }

            return [x, y];
        },
























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
         txt +=    s1 + 'Start: '        + d1s  + eol;
         txt +=    s1 + 'End:   '        + d2s  + eol;
         txt +=    s1 + 'Duration:     ' + dur  + eol;

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





function setupTestButtons()
{
   // get the container and set flex
   const container = document.getElementById('testButtons');

   const button1 = document.createElement('button');
   button1.textContent = 'Test 1';
   button1.setAttribute('id', 'testButton1');
   container.appendChild(button1);

   const button2 = document.createElement('button');
   button2.textContent = 'Test 2';
   button2.setAttribute('id', 'testButton2');
   container.appendChild(button2);


   const in1 = document.createElement('input');
   in1.setAttribute('id', 'in1');
   in1.setAttribute('type', 'range');
   in1.setAttribute('min', '10');
   in1.setAttribute('max', '20');
   in1.setAttribute('step', '1');
   container.appendChild(in1);


   // find color slider width slider
   const in1a = document.getElementById("in1");
   
   // set initial value
   in1a.value = 14;

   // listen for changes
   in1a.addEventListener("input", () =>
   {
      console.log("slider val:", in1a.value);   
      document.documentElement.style.setProperty('--testvar', `${in1a.value}px`);
   });



   document.getElementById("testButton1").addEventListener("click", function()
   {
      console.log("test button 1 pressed");   
   });
   document.getElementById("testButton2").addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
   });
}


function setupSettingsDialog()
{
   document.body.innerHTML += `
      <dialog id="settingsDialog">
         <form method="dialog">
            <div class="divSectionContainer ">
               <div class="divSectionTitleSectionFrame">
                  <div class="divSectionTitleSectionContainer">
                     <div class="divSectionTitleFrame">Settings</div>
                     <div class="timeline divSectionTitleFrameButtonsFrame">
                        <div>
                          <button value="ok">OK</button>
                        </div>
                     </div>
                  </div>
               </div>
               <div style="display:flex flex-direction:column">
                  <div class="controlRow">
                     <label>Color Slider Enable:</label>
                     <input id="colorSliderEnableCheckbox" type="checkbox">
                  </div>
                  <div class="controlRow">
                     <label for: "colorSliderWidthSlider">Color Slider Width:</label>
                     <input id="colorSliderWidthSlider" type="range" min=40 max=800>
                  </div>
                  <div class="controlRow">
                     <label for: "rangeTableTextSizeSlider">Range Table Text Size:</label>
                     <input id="rangeTableTextSizeSlider" type="range" min=0.5 max=1 step=0.01>
                  </div>
                  <div class="controlRow">
                     <label>Use Datatables:</label>
                     <input id="useDatatablesCheckbox" type="checkbox">
                  </div>
               </div>
            </div>
         </form>
      </dialog>`;


   const dialog = document.getElementById("settingsDialog");
   const openButton = document.getElementById("openSettings");

   // listen for button click to open settings dialog
   openButton.addEventListener("click", () => { dialog.showModal(); });


   // colorSliderWidthSlider
   const colorSliderWidthSlider = document.getElementById("colorSliderWidthSlider");
   colorSliderWidthSlider.value = colorSliderWidth;
   colorSliderWidthSlider.addEventListener("input", () =>   {      setColorSliderWidth(colorSliderWidthSlider.value);   });


   // colorSliderEnableCheckBox
   const colorSliderEnableCheckBox = document.getElementById("colorSliderEnableCheckbox");
   colorSliderEnableCheckBox.checked = colorSliderEnable;
   colorSliderEnableCheckBox.addEventListener("input", () =>   {      setColorSliderEnable(colorSliderEnableCheckBox.checked);   });


   // rangeTableTextSlider
   const rangeTableTextSizeSlider = document.getElementById("rangeTableTextSizeSlider");
   rangeTableTextSizeSlider.value = rangeTableTextSize;
   rangeTableTextSizeSlider.addEventListener("input", () =>   {      setRangeTableTextSize(rangeTableTextSizeSlider.value);   });

   // useDatatablesCheckbox
   const useDatatablesCheckbox = document.getElementById("useDatatablesCheckbox");
   useDatatablesCheckbox.checked = useDatatables;
   useDatatablesCheckbox.addEventListener("input", () =>   {      setUseDatatables(useDatatablesCheckbox.checked);   });


}




var myChart;
var data;
let categoryArray = [];

let timelineHeight = 400;
let timelineDataRangeOption = 0;
let timelineViewRangeOption = 0;
let colorSliderWidth = 60;
let colorSliderEnable = 0;
let rangeTableTextSize = 1.0;

let useDatatables = 1;






var currentSessionId;
var currentGmMuid;


function readVariablesFromSessionStorage()
{
   // attempt to read all variables from session storage
   timelineHeight          = JSON.parse(sessionStorage.getItem("timelineHeight"));
   timelineDataRangeOption = JSON.parse(sessionStorage.getItem("timelineDataRangeOption"));
   timelineViewRangeOption = JSON.parse(sessionStorage.getItem("timelineViewRangeOption"));
   colorSliderWidth        = JSON.parse(sessionStorage.getItem("colorSliderWidth"));
   colorSliderEnable       = JSON.parse(sessionStorage.getItem("colorSliderEnable"));
   rangeTableTextSize      = JSON.parse(sessionStorage.getItem("rangeTableTextSize"));
   useDatatables           = JSON.parse(sessionStorage.getItem("useDatatables"));
   currentSessionId        = JSON.parse(sessionStorage.getItem("currentSessionId"));
   currentGmMuid           = JSON.parse(sessionStorage.getItem("currentGmMuid"));

   // if null, set default values
   if (timelineHeight          === null) timelineHeight          = 400;
   if (timelineDataRangeOption === null) timelineDataRangeOption = 0;
   if (timelineViewRangeOption === null) timelineViewRangeOption = 0;
   if (colorSliderWidth        === null) colorSliderWidth        = 60;
   if (colorSliderEnable       === null) colorSliderEnable       = 0;
   if (rangeTableTextSize      === null) rangeTableTextSize      = 1.0;
   if (useDatatables           === null) useDatatables           = 1;
   if (currentSessionId        === null) fetchDataMostRecentSession();
   if (currentGmMuid           === null) fetchDataMostRecentGm();


   // initialize things associated with these variables
   setColorSliderWidth(colorSliderWidth);
   setColorSliderEnable(colorSliderEnable);
   setRangeTableTextSize(rangeTableTextSize);
}


function checkLoaded()
{
   if ((currentGmMuid !== null) && (currentSessionId !== null)) 
   {
      fetchDataTimeline();
      fetchDataSessionsTable();
      fetchDataCurrentSession();
      fetchDataGmTable();
      fetchDataCurrentGm();
   } 
   else
   {
      checkLoaded.count = (checkLoaded.count || 0) + 1;

      if (checkLoaded.count > 10)
      {
         console.log('aborting...');
         return;
      }     

      console.log(checkLoaded.count, ' - variables not set yet, wait 100ms and check again...');
      setTimeout(checkLoaded, 100); // Check again in 100ms
   }
}


// -----------------------------------------------
// ---   setup once the page has loaded   ---
// -----------------------------------------------
document.addEventListener('DOMContentLoaded', (event) =>
{
   readVariablesFromSessionStorage();
   setTimeout(checkLoaded, 0); // schedule the callback to run asynchronously after the current synchronous code finishes executing
   setupSettingsDialog();
   //setupTestButtons();
});



function setCurrentSession(id)
{
   currentSessionId = Number(id);
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
function setRangeTableTextSize(val)
{
   rangeTableTextSize = val;
   sessionStorage.setItem("rangeTableTextSize", JSON.stringify(rangeTableTextSize));
   document.documentElement.style.setProperty('--rangeTableTextSize', `${val}rem`);
}

function setUseDatatables(val)
{
   useDatatables = val;  
   sessionStorage.setItem("useDatatables", JSON.stringify(useDatatables));
   fetchDataSessionsTable();
   fetchDataGmTable();
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

   // pad to 12 char
   returnText = returnText.padStart(12);

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
   document.getElementById("dataRangeStart" ).innerHTML = "Start: "   + getDateString(min);
   document.getElementById("dataRangeEnd"   ).innerHTML = "End: "     + getDateString(max);
   document.getElementById("dataDuration"   ).innerHTML = "Duration:" + getDuration(min, max);
   document.getElementById("dataItemCount"  ).innerHTML = "Items: "   + String(data.length);
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
   document.getElementById("viewRangeStart" ).innerHTML = "Start: "   + getDateString(min);
   document.getElementById("viewRangeEnd"   ).innerHTML = "End: "     + getDateString(max);
   document.getElementById("viewDuration"   ).innerHTML = "Duration:" + getDuration(min, max);
   document.getElementById("viewItemCount"  ).innerHTML = "Items: "   + view_item_count;
}


function setViewRange()
{
   var minutes;

   switch (timelineViewRangeOption)
   {
      case 0: minutes = 0;     break;  // full
      case 1: minutes = 43200; break;  // month 
      case 2: minutes = 10080; break;  // week
      case 3: minutes = 1440;  break;  // day 
      case 4: minutes = 60;    break;  // hour
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

      // set to earliest
      // d2 = d1 + minutes * 60000;

      // set to latest
      d1 = d2 - minutes * 60000;

      myChart.dispatchAction({
         type: 'dataZoom',
         dataZoomIndex: 0, // Index of the dataZoom component to target
         startValue: d1,
         endValue: d2,
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
   const container = document.getElementById('dataRangeSelectContainer');

   // label element
   const selectLabel = document.createElement('label');
   selectLabel.textContent = 'Data Range:';

   // select element
   const newSelect = document.createElement('select');
   newSelect.setAttribute('id', 'dataRangeSelect');
   newSelect.classList.add('select', 'timelineSelect');

   // fill select element with options from array
   const selectArray = ['full', 'month', 'week', 'day', 'hour', 'sess'];
   
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
   const container = document.getElementById('viewRangeSelectContainer');

   // label element
   const selectLabel = document.createElement('label');
   selectLabel.textContent = 'View Range:';

   // select element
   const newSelect = document.createElement('select');
   newSelect.setAttribute('id', 'viewRangeSelect');
   newSelect.classList.add('select', 'timelineSelect');

   // fill select element with options from array
   const selectArray = ['full', 'month', 'week', 'day', 'hour'];

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
               <div style="height:100%">
                  <table class="timelineRangeTable">
                     <tr> 
                        <td class="timelineRangeTableTdTop" id="dataRangeSelectContainer"></td>
                        <td class="timelineRangeTableTdTop" id="dataRangeStart"></td>
                        <td class="timelineRangeTableTdTop" id="dataRangeEnd"></td>
                        <td class="timelineRangeTableTdTop" id="dataDuration"></td>
                        <td class="timelineRangeTableTdTop" id="dataItemCount"></td>
                     </tr> 
                     <tr> 
                        <td class="timelineRangeTableTdBottom" id="viewRangeSelectContainer"></td>
                        <td class="timelineRangeTableTdBottom" id="viewRangeStart"></td>
                        <td class="timelineRangeTableTdBottom" id="viewRangeEnd"></td>
                        <td class="timelineRangeTableTdBottom" id="viewDuration"></td>
                        <td class="timelineRangeTableTdBottom" id="viewItemCount"></td>
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

   setupTimelineDateRange();
   setupTimelineViewRange();


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
               setCurrentSession(id);
               fetchDataCurrentSession();
               fetchDataGmTable();
               fetchDataTimeline();
            }
         });
      });



      if (useDatatables)
      {
         new DataTable("#mySessionsTable",
         {
            colReorder: true,
            columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: '_all' } ],
            order: [  [0, 'desc'] ]
         }  );
      }



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

      if (useDatatables)
      {
         new DataTable("#myGmTable",
         {
            colReorder: true,
            columnDefs: [ {  className: 'dt-head-center dt-body-center', targets: '_all' } ],
            order: [  [0, 'desc'] ]
         }  );
      }
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

   setHue(h)           {  this.hue = Number(h);                       this.save();  }
   toggleMinimized()   {  this.minimized = Number(!this.minimized);   this.save();  }
   setMinimized(m)     {  this.minimized = Number(m);                 this.save();  }
   setView(v)          {  this.view = Number(v);                      this.save();  }

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

