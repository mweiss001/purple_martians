import { createControlGroup, createDiv, setupLabelSpan, setupButtonControl, setupCheckBoxControl, setupSliderControl, setCheckbox, setSpanInnerHTML, setSliderVal, insertControl} from './common.js';


export function updateServerSnapshotControls(data)
{
   if (!data) return;

   setCheckbox('ssEnableCheckBox', data[0].ss_enabled);
   
   setSpanInnerHTML('ss_time', data[0].ss_time);

   setSliderVal(    'ssPeriodSlider', data[0].ss_period);
   setSpanInnerHTML('ssPeriodSpan',   data[0].ss_period);

   setSliderVal(    'ssSizeSlider',   data[0].ss_size);
   setSpanInnerHTML('ssSizeSpan',     data[0].ss_size);

}

export function setupServerSnapshotControls(parentContainer)
{
   const container = createControlGroup('Server Snapshot Controls', 'ssControlsContainer', '#6600FF');
   parentContainer.appendChild(container);
   
   setupControlsSsEnable(container);
   setupControlsSsPeriod(container);
   setupLabelSpan(container, 'ss_time', 'Generate Time:', 0);
   setupControlsSsSize(container);
   setupControlsSsSingle(container);   
   
}

function setupControlsSsEnable(parentContainer)
{
   setupCheckBoxControl(parentContainer, 'ssEnableCheckBox', 'Enable:', 0);

   document.getElementById('ssEnableCheckBox').addEventListener("input", () => 
   {  
      insertControl("ss_enable", 0, 0);
   });

}
function setupControlsSsSingle(parentContainer)
{
   const button = document.createElement('button');
   button.textContent = 'Single';
   button.setAttribute('id', 'ssSingleButton');

   parentContainer.appendChild(button);
   button.addEventListener("click", function()
   {
      insertControl("ss_single", 0, 0);
   });
}


function setupControlsSsPeriod(parentContainer)
{
   setupSliderControl(parentContainer, 'ssPeriodSlider', 'ssPeriodSpan', 'SS period:', 0, 80, 40, 0);
   const slider = document.getElementById('ssPeriodSlider');
   const span   = document.getElementById('ssPeriodSpan');

   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
      insertControl("ss_period", slider.value, 0);
   });
}

function setupControlsSsSize(parentContainer)
{
   setupSliderControl(parentContainer, 'ssSizeSlider', 'ssSizeSpan', 'SS size:', 100, 2000, 100, 100);
   const slider = document.getElementById('ssSizeSlider');
   const span   = document.getElementById('ssSizeSpan');

   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
      insertControl("ss_size", slider.value, 0);
   });
}








