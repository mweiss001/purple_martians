import format             from './node_modules/date-fns/format.js';
import formatDuration     from './node_modules/date-fns/formatDuration.js';
import intervalToDuration from './node_modules/date-fns/intervalToDuration.js';
import differenceInDays   from './node_modules/date-fns/differenceInDays.js';


var dom = document.getElementById('chart-container');
var myChart = echarts.init(dom, null, {
  renderer: 'canvas',
  useDirtyRect: false
});
var app = {};

var option;

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
      1768849044000,
      1768849051000,
      0,
      0,
      "gm",
      "EOBUDITDIJQASENJ",
      80
    ]
  },
  {
    "value": [
      1768849056000,
      1768849062000,
      0,
      0,
      "gm",
      "KBMSXEJDVEPMYRHK",
      80
    ]
  },
  {
    "value": [
      1768849067000,
      1768849072000,
      0,
      0,
      "gm",
      "KWIVQARUGPIGERNM",
      80
    ]
  },
  {
    "value": [
      1768849038000,
      1768849077000,
      1,
      9,
      "Forest",
      "Forest",
      1
    ]
  }
];




option =
{


   xAxis: { type: "time", },
   yAxis: { type: "category", },
   series:
   [
      {
         type: "custom",
			renderItem: (params, api) =>
         {              
            // api.value() is the data!! 0-start 1-end 2-category

            var catIndex = api.value(2);
            var timeSpan = [api.value(0), api.value(1)];

//            var start = api.coord([timeSpan[0], maxCat - catIndex]);
//            var end = api.coord([timeSpan[1], maxCat -catIndex]);

            var start = api.coord([timeSpan[0], catIndex]);
            var end = api.coord([timeSpan[1], catIndex]);


            var size = api.size([0,1]);

            // relative height of the bars  
            var height = size[1] * 0.6;

            // actual timespan in seconds 
            var ts = (api.value(1) - api.value(0)) / 1000;

            // calculate width
            var width = end[0] - start[0];
            if (width < 2) end[0] = start[0] + 2;
                 
//                 console.log("catIndex:%d - seconds:%d  width: ", catIndex, ts, width);
//                 console.log("start   :", start);
//                 console.log("end     :", end);
//                 console.log("width   :", width);

            var rect = echarts.graphic.clipRectByRect
            (
               { x: start[0], y: start[1] - height / 2, width: end[0] - start[0], height: height},
               { x: params.coordSys.x, y: params.coordSys.y, width: params.coordSys.width, height: params.coordSys.height}
            );
            return(
               rect &&
               {
                  type: "rect",
                  transition: ["shape"],
                  shape: rect,
                  style: { fill: playerColors[api.value(3)], },
               }
            );
         },
         encode: { x: [0,1], y: 0, },
      }
   ],

/*
   tooltip:
   {
        trigger: 'axis',
        axisPointer: { // Style the axis pointer line and shadow
            type: 'cross',
            lineStyle: { color: '#555', type: 'dashed' }
        },

   },
*/




   tooltip:
   {

      backgroundColor: 'rgba(50, 50, 50, 0.7)',
      borderColor: '#333',
      textStyle: { color: '#fff',
                   fontFamily: 'monospace',
//                   fontSize: 16,
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
         const fst = format(d1, 'yyyy-MM-dd hh:mm:ss');
         const fet = format(d2, 'yyyy-MM-dd hh:mm:ss');

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






/*
      axisLabel:
      {
         formatter: function (value)
         {
           return echarts.format.formatTime('{yyyy}-{MM}-{dd}', value);
         }
      }

*/

   },
   dataZoom:
   [
      {
         type: "slider",
         filterMode: "none"
      },
   ],
}



function getDuration(d1, d2)
{
   const duration = intervalToDuration({ start: d1, end: d2 });
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
      returnText = differenceInDays(d2, d1) + " ";
   }
  returnText += zeroPad(duration.hours) + ":" + zeroPad(duration.minutes) + ":" + zeroPad(duration.seconds);

   // pad to 16 char
   returnText = returnText.padStart(16);

   return returnText;
}





if (option && typeof option === 'object')
{
   myChart.setOption(option);
}

fetchData('data.php');

window.addEventListener('resize', myChart.resize);

document.getElementById("fetchDataButton").addEventListener("click", function()
{
   fetchData('data.php');
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



// iterate data and finds min start and max end
// returns range array [0]-min [1]-max
function getDataRange(dat)
{
   // initial values are the first in the array
   var min = dat[0].value[0];
   var max = dat[0].value[1];
  
   // iterate the array
   for (let i = 1; i < dat.length; i++)
   {
      if (dat[i].value[0] < min) min = dat[i].value[0];
      if (dat[i].value[1] > max) max = dat[i].value[1];
   }
   // console.log("min:", min.toString(), "\nmax:", max.toString() );   
   var range = [min, max];
   return range;
}


// iterate data and setup categories
function setCategories(dat)
{
   // clear the category array
   catArr.length = 0;

   // iterate the data array
   for (let i = 0; i < dat.length; i++)
   {
      // get name
      var name = dat[i].value[4];

      // check if name already exists in catArr and get index if it does
      var catIndex = catArr.indexOf(name);
      
      // does not exist, create it and get index     
      if (catIndex === -1) catIndex = catArr.push(name) - 1;

      // set category index in data  
      dat[i].value[2] = catIndex;
   }

   maxCat = catArr.length-1; 
}



function reloadChart()
{
   // modify the array, add key-value 'name' and name from value 4
   data = data.map(obj => { return { name: obj.value[4], ...obj  }; });


   var range = getDataRange(data);


   setCategories(data);


   myChart.setOption(
   {
      yAxis: { data: catArr },
      xAxis: { min: range[0], max: range[1] },
      series: [ { data: data } ]
   });
}


async function fetchData(url)
{
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






















