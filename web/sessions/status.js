import { setupTopPageLinks, getDHMSfromFrame } from './common.js';
import { setupClientStatusControls,   updateClientStatusControls}   from './clientStatusControls.js';
import { setupServerSnapshotControls, updateServerSnapshotControls} from './serverSnapshotControls.js';
import { setupServerControls,         updateServerControls}         from './serverControls.js';



document.addEventListener('DOMContentLoaded', (event) =>
{
   setupTopPageLinks();
   const statusControls = document.getElementById('statusControls');
   statusControls.style.display = 'flex';
   setupServerSnapshotControls(statusControls);
   setupServerControls(statusControls);

   fetchDataServerStatus();

   document.getElementById('ssSingleButton').addEventListener("click", function()
   {
      setTimeout(() =>
      {
         fetchDataServerStatus();
      }, 200);
   });

   document.getElementById("refreshButton").addEventListener("click", function()
   {
      fetchDataServerStatus();
   });
});


async function fetchDataServerStatus()
{
   var data = [];
   try
   {
      const response = await fetch('fetchDataServerStatus.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      data = await response.json();
   } catch (error) { console.error("Fetch error:", error.message);
   } finally
   {
      updateStatusText(data);
      updateLevelText(data);
      updateClientSection(data);
      updateLevelIcon(data);
      updateServerControls(data);
      updateServerSnapshotControls(data);
   }
}

function updateStatusText(data)
{
   const statusText = document.getElementById('statusText');
   statusText.innerHTML  =   "Server Version : " + data[0].version;
   statusText.innerHTML += "\nLast Update    : " + data[0].timestamp;
   statusText.innerHTML += "\nServer Uptime  : " + getDHMSfromFrame(data[0].uptime*40);
   statusText.innerHTML += "\nServer Load    : " + data[0].cpua;
}

function updateLevelText(data)
{
   const levelText = document.getElementById('levelText');
   levelText.innerHTML  =   "Level   : " + data[0].level;
   levelText.innerHTML += "\nTime    : " + getDHMSfromFrame(data[0].level_time);
   levelText.innerHTML += "\nMoves   : " + data[0].moves;
   levelText.innerHTML += "\nEnemies : " + data[0].enemies;
}

function updateClientSection(data)
{
   if (data[0].clients === 0) document.getElementById('currentClientsTitle').innerHTML = 'No Current Clients';
   else fetchDataServerStatusClients();
}

async function fetchDataServerStatusClients()
{
   document.getElementById('currentClientsTitle').innerHTML = 'Current Clients';
   try
   {
      const response = await fetch('fetchDataServerStatusClients.php');
      if (!response.ok) { throw new Error(`HTTP error! Status: ${response.status}`); }
      document.getElementById('currentClientsTable').innerHTML = await response.text();
   } catch (error) { console.error("Fetch error:", error.message); }
}

// this will force image reload
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

function updateLevelIcon(data)
{
   // get server snapshot level icon
   if (data[0].ss_enabled) reloadImageById('levelIcon', '/downloads/lev_stat.png');
   else
   {
      // get static level icon 
      const iconpath = "/assets/icons/lev" + String(data[0].level).padStart(3, '0') + ".png";
      document.getElementById('levelIcon').src = iconpath;
   }
   const size = data[0].ss_size + 'px'; 
   levelIcon.style.height = size;
   levelIcon.style.width  = size;
}
