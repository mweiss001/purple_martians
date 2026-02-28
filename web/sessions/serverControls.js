import { createControlGroup, createDiv, setupLabelSpan, setupButtonControl, setupCheckBoxControl, setupSliderControl, setCheckbox, setSpanInnerHTML, setSliderVal, insertControl} from './common.js';

export function updateServerControls(data)
{
   if (!data) return;
   setCheckbox('fakekeyCheckBox',  data[0].fakekey);
}

export function setupServerControls(parentContainer)
{
   const container = createControlGroup('Server Controls', 'sControlsContainer', '#6600FF');
   parentContainer.appendChild(container);
   setupControlsFakekey(container);
}



function setupControlsFakekey(parentContainer)
{
   setupCheckBoxControl(parentContainer, 'fakekeyCheckBox', 'Fake Key:', 0);
   document.getElementById('fakekeyCheckBox').addEventListener("input", () => 
   {  
      insertControl("fakekey", 0, 0);
   });
}






