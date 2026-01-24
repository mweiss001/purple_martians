

var dom = document.getElementById('chart-container');
var myChart = echarts.init(dom, 'dark',
{
   renderer: 'canvas',
   useDirtyRect: false
});


window.addEventListener('resize', myChart.resize);

// range selector
const rangeSelect = document.getElementById("range-select");
rangeSelect.selectedIndex = 0;
rangeSelect.addEventListener("change", (event) => { fetchData(); });


// load initial
fetchData();



document.getElementById("fetchDataButton").addEventListener("click", function()
{
   fetchData();
});

document.getElementById("testSet1Button").addEventListener("click", function()
{
   data = data1;
   reloadChart();
});

document.getElementById("testSet2Button").addEventListener("click", function()
{
   data = data2;
   reloadChart();
});

document.getElementById("testSet3Button").addEventListener("click", function()
{
   data = data3;
   reloadChart();   
});


document.getElementById("testButton").addEventListener("click", function()
{
   test();
});





var data;
var maxCat;
let catArr = [];

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


let data3 =
[
  {
    "value": [
      1768891742000,
      1768891751000,
      0,
      0,
      "gm",
      "ZSQNNSBKMRXRZBDK",
      80
    ]
  },
  {
    "value": [
      1768891762000,
      1768891787000,
      0,
      0,
      "gm",
      "TGTDDQKWRXDCSKVY",
      80
    ]
  },
  {
    "value": [
      1768891805000,
      1768891827000,
      0,
      0,
      "gm",
      "TBAUSROTBOCSSVEL",
      37
    ]
  },
  {
    "value": [
      1768891724000,
      1768891730000,
      1,
      9,
      "Forest",
      "Forest",
      1
    ]
  },
  {
    "value": [
      1768891736000,
      1768891792000,
      2,
      9,
      "Forest",
      "Forest",
      2
    ]
  },
  {
    "value": [
      1768891757000,
      1768891765000,
      3,
      7,
      "jenny!",
      "jenny!",
      3
    ]
  },
  {
    "value": [
      1768891768000,
      1768891835000,
      4,
      7,
      "jenny!",
      "jenny!",
      4
    ]
  },
  {
    "value": [
      1768891777000,
      1768891831000,
      5,
      4,
      "Zaiden",
      "Zaiden",
      5
    ]
  },
  {
    "value": [
      1768891796000,
      1768891839000,
      6,
      9,
      "Forest",
      "Forest",
      6
    ]
  }
];







let data2 =
[
  {
    "value": [
      1768891496000,
      1768891502000,
      0,
      0,
      "gm",
      "SRZQFLTUNHZAXGQZ",
      80
    ]
  },
  {
    "value": [
      1768891537000,
      1768891547000,
      0,
      0,
      "gm",
      "WQBKMFETVCKZBKBT",
      80
    ]
  },
  {
    "value": [
      1768891555000,
      1768891577000,
      0,
      0,
      "gm",
      "QNLHHDEKXEMQGHBK",
      80
    ]
  },
  {
    "value": [
      1768891489000,
      1768891579000,
      1,
      9,
      "Forest",
      "Forest",
      1
    ]
  },
  {
    "value": [
      1768891530000,
      1768891558000,
      2,
      7,
      "jenny!",
      "jenny!",
      2
    ]
  },
  {
    "value": [
      1768891566000,
      1768891577000,
      3,
      7,
      "jenny!",
      "jenny!",
      3
    ]
  }
];



let data1 =

[
  {
    "value": [
      1769090641000,
      1769090673000,
      0,
      0,
      "gm",
      "MIAGCXYHSZZRMJWX",
      80
    ]
  },
  {
    "value": [
      1769090704000,
      1769090727000,
      0,
      0,
      "gm",
      "XSDXTBRIVFBWRHOL",
      37
    ]
  },
  {
    "value": [
      1769090482000,
      1769090742000,
      1,
      6,
      "Tandoori",
      "Tandoori",
      1
    ]
  },
  {
    "value": [
      1769090524000,
      1769090664000,
      2,
      13,
      "Aqualung",
      "Aqualung",
      2
    ]
  },
  {
    "value": [
      1769090555000,
      1769090649000,
      3,
      9,
      "Forest",
      "Forest",
      3
    ]
  },
  {
    "value": [
      1769090592000,
      1769090742000,
      4,
      7,
      "jenny!",
      "jenny!",
      4
    ]
  },
  {
    "value": [
      1769090632000,
      1769090742000,
      5,
      8,
      "michael",
      "michael",
      5
    ]
  },
  {
    "value": [
      1769090655000,
      1769090742000,
      3,
      9,
      "Forest",
      "Forest",
      6
    ]
  },
  {
    "value": [
      1769090669000,
      1769090742000,
      2,
      13,
      "Aqualung",
      "Aqualung",
      7
    ]
  }
];





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


            var name1 = api.value(4); // player name
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
                     },
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

      backgroundColor: 'rgba(50, 50, 50, 0.9)',
      borderColor: '#aaaaaa',
      borderWidth: 1,


      textStyle:
      {
         color: '#fff',
         fontFamily: 'monospace',
//                  fontSize: 16,
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
            name += '</span><br>';
         }
         else // type = gm
         {
            var lev = params.data.value[6];
            var l2 = String(lev).padStart(3, '0');
            icon = `/assets/icons/lev${l2}.png`;
            name = '<img src="' + icon + '" style="width: 50px; height: 50px; vertical-align: middle;">';
            name += '<span style="white-space:pre"> ';
            name += `Level:${lev}`; // level number
            name += '</span><br>';
         }            

         // get date objects fron start and end
         const d1 = new Date(params.data.value[0]);
         const d2 = new Date(params.data.value[1]);

         // format start and end
         const fst = dateFns.format(d1, 'yyyy-MM-dd HH:mm:ss');
         const fet = dateFns.format(d2, 'yyyy-MM-dd HH:mm:ss');

         // get duration
         const dur =  getDuration(d1, d2);

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

// initial options load for chart
if (option && typeof option === 'object') myChart.setOption(option);
















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
   // initial values are the first in the array
   var min = data[0].value[0];
   var max = data[0].value[1];
  
   // iterate the array
   for (let i = 1; i < data.length; i++)
   {
      if (data[i].value[0] < min) min = data[i].value[0];
      if (data[i].value[1] > max) max = data[i].value[1];
   }
   // console.log("min:", min.toString(), "\nmax:", max.toString() );   
   var range = [min, max];
   return range;
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



function reloadChart()
{
   if (data.length == 0)
   {
      console.log("No data");
      catArr.length = 0;
      maxCat = 0;
      range = [0,0];
   }
   else
   {
      // modify the array, add key-value 'name' and name from value 4
      data = data.map(obj => { return { name: obj.value[4], ...obj  }; });
      var range = getDataRange();
      setCategories();
   }
   myChart.setOption(
   {
      yAxis: { data: catArr },
      xAxis: { min: range[0], max: range[1] },
      series: [ { data: data } ]
   }, false);
}


async function fetchData()
{
   if (data !== undefined) data.length = 0;

   var url = 'data.php';

   const val = rangeSelect.selectedIndex;
   if (val == 0) url = 'data.php?range=all'; 
   if (val == 1) url = 'data.php?range=min&min=60';
   if (val == 2) url = 'data.php?range=min&min=1440';
   if (val == 3) url = 'data.php?range=session';

   try
   {
      const response = await fetch(url);
      if (!response.ok)
      { // Check if the response status is in the 200-299 range
         throw new Error(`HTTP error! Status: ${response.status}`);
      }
      data = await response.json(); // Parse the response body as JSON
      console.log(data);
      reloadChart();

   } catch (error)
   {
      console.error("Fetch error:", error.message); // Handles network errors or the error thrown above
   }
}






















