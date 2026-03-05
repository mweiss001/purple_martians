import { createControlGroup, createDiv, setupLabelSpan, setupButtonControl, setupCheckBoxControl, setupSliderControl, setCheckbox, setSpanInnerHTML, setSliderVal, insertControl} from './common.js';

export function setupClientStatusChartSelectControls(parentContainer, numCharts, chartMap, chartHeight)
{
   const container = createControlGroup('Client Status Chart Select Controls', 'clientStatusChartSelectControlsContainer', '#6600FF');
   parentContainer.appendChild(container);
  
   setupClientStatusChartHeight(container, chartHeight);
  
   setupClientStatusPresets(container);

   setupClientStatusNumChart(container, numCharts);

   for (var i=0; i<8; i++)
      setupClientStatusChartType(container, i, chartMap[i]);
  

  
}


export function updateClientStatusChartSelectControls(numCharts, chartMap, chartHeight)
{
   var slider = document.getElementById('numChartsSlider');
   var span   = document.getElementById('numChartsSpan');
   span.innerHTML = slider.value = numCharts;

   slider = document.getElementById('chartHeightSlider');
   span   = document.getElementById('chartHeightSpan');
   span.innerHTML = slider.value = chartHeight;

   for (var i=0; i<8; i++)
   {
      // get container
      var contId = "chartTypeSelectContainer" + i;      
      var cont = document.getElementById(contId);
      
      if (i < numCharts)
      {
         cont.style.display = 'flex';
         var selectId = "chartTypeSelect" + i;      
         const sel = document.getElementById(selectId);
         sel.selectedIndex = chartMap[i];
      }
      // hide controls for charts that not active
      else cont.style.display = 'none';
   }
}





function setupClientStatusPresets(parentContainer)
{
   // label element
   const newSelectLabel = document.createElement('label');
   newSelectLabel.textContent = "Presets: ";         
   
   // select element
   const newSelect = document.createElement('select');
   var selectId = "chartPresetSelect";      
   newSelect.setAttribute('id', selectId);

   // fill select element with options from array
   const types = ['default', 'minimal', 'all'];

   var indx = 0; 
   for (const type of types)
   {
      const newOption = document.createElement('option');
      newOption.setAttribute('value', indx);
      newOption.textContent = type;
      newSelect.appendChild(newOption);
      indx++;
   }

   // add select to label 
   newSelectLabel.appendChild(newSelect);

   // add label to parent container   
   parentContainer.appendChild(newSelectLabel);

}





function setupClientStatusChartType(parentContainer, index, type)
{
   const container = createDiv();
   var contId = "chartTypeSelectContainer" + index;      
   container.setAttribute('id', contId);


   // label element
   const newSelectLabel = document.createElement('label');
   newSelectLabel.textContent = "Chart " + index + " ";         
   
   // select element
   const newSelect = document.createElement('select');
   var selectId = "chartTypeSelect" + index;      
   newSelect.setAttribute('id', selectId);

   // fill select element with options from array
   const types = ['cpu', 'sync', 'ping', 'lcor', 'rcor', 'rewind', 'difs', 'tkbs'];

   var indx = 0; 
   for (const type of types)
   {
      const newOption = document.createElement('option');
      newOption.setAttribute('value', indx);
      newOption.textContent = type;
      newSelect.appendChild(newOption);
      indx++;
   }

   // set initial index 
   newSelect.selectedIndex = type;

   // add select to label 
   newSelectLabel.appendChild(newSelect);

   // add label to container   
   container.appendChild(newSelectLabel);

   // add container to parent container   
   parentContainer.appendChild(container);



}


function setupClientStatusNumChart(parentContainer, numCharts)
{
   setupSliderControl(parentContainer, 'numChartsSlider', 'numChartsSpan', 'Number of Charts:', 1, 8, 1, 1);
   const slider = document.getElementById('numChartsSlider');
   const span   = document.getElementById('numChartsSpan');

   // set initial value
   span.innerHTML = slider.value = numCharts;

   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
   });
}


function setupClientStatusChartHeight(parentContainer, chartHeight)
{
   setupSliderControl(parentContainer, 'chartHeightSlider', 'chartHeightSpan', 'Chart Height:', 100, 400, 20, 300);
   const slider = document.getElementById('chartHeightSlider');
   const span   = document.getElementById('chartHeightSpan');

   // set initial value
   span.innerHTML = slider.value = chartHeight;

   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
   });
}






