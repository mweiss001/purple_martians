import { createControlGroup, createDiv, setupLabelSpan, setupButtonControl, setupCheckBoxControl, setupSliderControl, setCheckbox, setSpanInnerHTML, setSliderVal, insertControl} from './common.js';



export function setupClientStatusControls(parentContainer)
{
   const container = createControlGroup('Client Status Controls', 'clientStatusControlsContainer', '#0066FF');

   container.style.width = '360px';

//   container.style.height = '1660px';

   parentContainer.appendChild(container);

   setupLabelSpan(container, 'numRowsSpan', "Num Rows:", '');

   setupUpdateNumRowsButton(container);
   setupDeleteRows10Button(container);

   setupClientStatusInsertEnable(container);
   
   setupLabelSpan(container, 'sicsb_size_targetSpan', "Batch Size Target:", '');
   setupLabelSpan(container, 'sicsb_size_actualSpan', "Batch Size Actual:", '');

   setupLabelSpan(container, 'sicsb_time_avgSpan', "Batch Time Avg:", '');
   setupLabelSpan(container, 'sicsb_time_maxSpan', "Batch Time Max:", '');
   
   setupClientStatusTargetBatchSize(container);
   
}


export function updateClientStatusControls(data)
{
   if (!data) return;
   setCheckbox('sics_enableCheckBox', data[0].sics_enable);
   setSpanInnerHTML('sicsb_size_targetSpan', data[0].sicsb_size_target);
   setSpanInnerHTML('sicsb_size_actualSpan', data[0].sicsb_size_actual);
   setSpanInnerHTML('sicsb_time_avgSpan',    data[0].sicsb_time_avg);
   setSpanInnerHTML('sicsb_time_maxSpan',    data[0].sicsb_time_max);
}



function setupClientStatusInsertEnable(parentContainer)
{
   setupCheckBoxControl(parentContainer, 'sics_enableCheckBox', 'Insert Enable:', '');

   document.getElementById('sics_enableCheckBox').addEventListener("input", () => 
   {  
      insertControl("server_insert_client_status_enable", 0, 0);
   });

}



function setupClientStatusTargetBatchSize(parentContainer)
{
   setupSliderControl(parentContainer, 'sicsb_size_targetSlider', 'sicsb_size_targetSpan2', 'Batch Size:', 1, 200, 1, 20);

   const slider = document.getElementById('sicsb_size_targetSlider');
   const span   = document.getElementById('sicsb_size_targetSpan2');

   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
      insertControl("server_insert_client_status_batch_size_target", slider.value, 0);
   });

}


function setupUpdateNumRowsButton(parentContainer)
{
   setupButtonControl(parentContainer, 'updateNumRowsButton', 'Update');
   document.getElementById('updateNumRowsButton').addEventListener("click", function() {  fetchNumRows(); });
}

function setupDeleteRows10Button(parentContainer)
{
   setupButtonControl(parentContainer, 'deleteRows10Button', 'Delete 10%');
   document.getElementById('deleteRows10Button').addEventListener("click", function() {  deleteRows10(); });
}



async function fetchNumRows()
{
   try
   {
      var url = 'findStatusNumRows.php';
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      document.getElementById('numRowsSpan').innerHTML = await response.text();
   } catch (error) { console.error("Fetch error:", error.message); }
}


async function deleteRows10()
{
   try
   {
      var url = 'deleteStatusRows.php';
      const response = await fetch(url);
      if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`); // Check if the response status is in the 200-299 range
      document.getElementById('numRowsSpan').innerHTML = await response.text();
   } catch (error) { console.error("Fetch error:", error.message); }
}






