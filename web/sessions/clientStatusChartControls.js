import { createControlGroup, createDiv, setupLabelSpan, setupButtonControl, setupCheckBoxControl, setupSliderControl, setCheckbox, setSpanInnerHTML, setSliderVal, insertControl} from './common.js';

export function setupClientStatusChartControls(parentContainer, updateInterval, chartTimeRange)
{
   const container = createControlGroup('Client Status Chart Controls', 'clientStatusChartControlsContainer', '#6600FF');
   parentContainer.appendChild(container);

   setupIntervalControl(container, updateInterval);

   setupLabelSpan(container, 'actualIntervalSpan',   'Actual Interval:',    '');
   setupLabelSpan(container, 'fetchTimeSpan',        'Fetch Time:',         '');
   setupLabelSpan(container, 'fetchNumFramesSpan',   'Fetch Frames:',       '');
   setupLabelSpan(container, 'fetchNumRowsSpan',     'Fetch Rows:',         '');
   
   setupRangeControl(container, chartTimeRange);

   setupLabelSpan(container, 'arrayProcessTimeSpan', 'Array Process Time:', '');
   setupLabelSpan(container, 'updateChartTimeSpan',  'Update Chart Time:',  '');
   setupLabelSpan(container, 'pileUpSpan',           'Pile Up:',            '');

   setupStartStopButton(container);

}


function setupStartStopButton(parentContainer)
{
   const stopButton = document.createElement('button');
   stopButton.textContent = 'Stop';
   stopButton.setAttribute('id', 'stopButton');
   parentContainer.appendChild(stopButton);
}

function setupIntervalControl(parentContainer, updateInterval)
{
   setupSliderControl(parentContainer, 'intervalSlider', 'intervalSliderSpan', 'Refresh Interval:', 25, 2000, 25, updateInterval);
   const slider = document.getElementById('intervalSlider');
   const span   = document.getElementById('intervalSliderSpan');
   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
   });
}

function setupRangeControl(parentContainer, chartTimeRange)
{
   setupSliderControl(parentContainer, 'rangeSlider', 'rangeSliderSpan', 'Range Control:', 1, 60, 1, chartTimeRange);
   const slider = document.getElementById('rangeSlider');
   const span   = document.getElementById('rangeSliderSpan');
   slider.addEventListener("input", () =>
   {
      span.innerHTML = slider.value;
   });
}
