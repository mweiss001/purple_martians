


// -----------------------------------------------
// ---   setup once the page has loaded   ---
// -----------------------------------------------
document.addEventListener('DOMContentLoaded', (event) =>
{
   fetchDataServerStatus();

   document.getElementById("refreshButton").addEventListener("click", function()
   {
      fetchDataServerStatus();
   });
});



function getDHMSfromFrame(frame)
{
   const duration = dateFns.intervalToDuration({ start: 0, end: frame*25 });

   var returnText = "";
   const zeroPad = (num) => String(num).padStart(2, '0');

   const parts = [
      duration.days,
      duration.hours,
      duration.minutes,
      duration.seconds
   ];

   if (duration.days !== undefined)
   {
      // this will only be the remaining days after accounting for full years and months, get actual days here
      returnText = dateFns.differenceInDays(frame*25, 0) + " ";
   }

   if (duration.hours   === undefined) duration.hours   = 0;
   if (duration.minutes === undefined) duration.minutes = 0;
   if (duration.seconds === undefined) duration.seconds = 0;

   if (duration.hours   !== 0) returnText += zeroPad(duration.hours)   + ":";
   if (duration.minutes !== 0) returnText += zeroPad(duration.minutes) + ":";
   returnText += zeroPad(duration.seconds);

   return returnText;
}





function reloadImageById(imageId, imageUrl)
{
   const imgElement = document.getElementById(imageId);
   if (imgElement)
   {
      // Append a unique timestamp to the original URL
      const newSrc = imageUrl + '?' + new Date().getTime();
      imgElement.src = newSrc;
   }
}


async function fetchDataServerStatus()
{
   const size = '1000px'; 
   const levelIcon = document.getElementById('levelIcon');
   levelIcon.style.height = size;
   levelIcon.style.width =  size;
   //levelIcon.src = '/downloads/lev_stat.png';

   // this will force image reload
   reloadImageById('levelIcon', '/downloads/lev_stat.png');


   var data = [];
   try
   {
      const response = await fetch('fetchDataServerStatus.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      data = await response.json();
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      const statusText = document.getElementById('statusText');
      statusText.innerHTML  =   "Server Version : " + data[0].version;
      statusText.innerHTML += "\nLast Update    : " + data[0].timestamp;
      statusText.innerHTML += "\nServer Uptime  : " + getDHMSfromFrame(data[0].uptime*40);
      statusText.innerHTML += "\nServer Load    : " + data[0].cpu;

 
      const levelText = document.getElementById('levelText');
      levelText.innerHTML  =   "Level   : " + data[0].level;
      levelText.innerHTML += "\nTime    : " + getDHMSfromFrame(data[0].level_time);
      levelText.innerHTML += "\nMoves   : " + data[0].moves;
      levelText.innerHTML += "\nEnemies : " + data[0].enemies;






      if (data[0].clients === 0) 
      {
         document.getElementById('currentClientsTitle').innerHTML = 'No Current Clients';
      }
      else
      {
         document.getElementById('currentClientsTitle').innerHTML = 'Current Clients';
         fetchDataServerStatusClients();
      }
   }
}


async function fetchDataServerStatusClients()
{
   try
   {
      const response = await fetch('fetchDataServerStatusClients.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      document.getElementById('currentClientsTable').innerHTML = await response.text();
   } catch (error) { console.error("Fetch error:", error.message); }
}





