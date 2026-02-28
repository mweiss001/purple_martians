// common.js

// share variables and functions

export const playerColors = [
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

export function setupTestButtons()
{
   const button1 = document.createElement('button');
   button1.textContent = 'Test 1';
   button1.setAttribute('id', 'testButton1');
   testButtons.appendChild(button1);

   const button2 = document.createElement('button');
   button2.textContent = 'Test 2';
   button2.setAttribute('id', 'testButton2');
   testButtons.appendChild(button2);

   const in1 = document.createElement('input');
   in1.setAttribute('id', 'in1');
   in1.setAttribute('type', 'range');
   in1.setAttribute('min', '10');
   in1.setAttribute('max', '20');
   in1.setAttribute('step', '1');
   testButtons.appendChild(in1);
  
   // set initial value
   in1.value = 14;
   // listen for changes
   in1.addEventListener("input", () =>
   {
      console.log("slider val:", in1.value);   
   });

   button1.addEventListener("click", function()
   {
      console.log("test button 1 pressed");   
   });
   button2.addEventListener("click", function()
   {
      console.log("test button 2 pressed");   
   });
}


export function getDHMSfromFrame(frame)
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


export function setupTopPageLinks()
{
   topPageLinks.innerHTML = `
      <a href="../index.html"  class="button linksButton" >Back to Main Page</a>
      <a href="sessions.html"  class="button linksButton" >Sessions</a>
      <a href="status.html"    class="button linksButton" >Status</a>
      <a href="s2.html"        class="button linksButton" >S2</a> `;
}






// -------------------------------------------------
// - common control functions
// -------------------------------------------------

export async function insertControl(key, val, mod)
{
   try { fetch(`insertStatusControl.php?key=${key}&val=${val}&mod=${mod}`);
   } catch (error) { console.error("Fetch error:", error.message);
   } finally { }
}

export function setCheckbox(id, val)
{
   const cb = document.getElementById(id);
   if (cb) cb.checked = val;
}

export function setSpanInnerHTML(id, val)
{
   const span = document.getElementById(id);
   if (span) span.innerHTML = val;
}

export function setSliderVal(id, val)
{
   const slider = document.getElementById(id);
   if (slider) slider.value = val;
}


export function createControlGroup(title, id, color)
{
   const container = document.createElement('div');
   container.setAttribute('id', id);
   container.style.display = 'flex';
   container.style.flexDirection = 'column';
   container.style.gap = '4px';
   container.style.alignItems = 'center';
   container.style.border = `1px solid ${color}`;

   const label = document.createElement('label');
   label.textContent = title;
   container.appendChild(label);

   return container;   
}

export function createDiv()
{
   const container = document.createElement('div');
   container.style.display = 'flex';
   container.style.gap = '4px';
   container.style.alignItems = 'center';
   container.style.border = '1px solid #8000FF';
   return container;   
}

export function setupLabelSpan(parentContainer, spanId, labelText, initialValue)
{
   const container = createDiv();
   container.style.justifyContent = 'right';

   // label element
   const label = document.createElement('label');
   label.textContent = labelText;
   container.appendChild(label);

   // span element
   const span = document.createElement('span');
   span.setAttribute('id', spanId);
   span.style.width = '40px';
   span.innerHTML = initialValue;
   container.appendChild(span);

   parentContainer.appendChild(container);
}


export function setupButtonControl(parentContainer, buttonId, buttonText)
{
   const container = createDiv();

   // button element
   const button = document.createElement('button');
   button.setAttribute('id', buttonId);
   button.textContent = buttonText;
   container.appendChild(button);
   parentContainer.appendChild(container);
}


export function setupSliderControl(parentContainer, sliderId, spanId, labelText, min, max, step, initialValue)
{
   const container = createDiv();

   // slider input element
   const slider = document.createElement('input');
   slider.setAttribute('id', sliderId);
   slider.setAttribute('type', 'range');
   slider.setAttribute('min', min);
   slider.setAttribute('max', max);
   slider.setAttribute('step', step);
   slider.value = initialValue;
   container.appendChild(slider);

   // label element
   const label = document.createElement('label');
   label.textContent = labelText;
   container.appendChild(label);

   // span element
   const span = document.createElement('span');
   span.setAttribute('id', spanId);
   span.innerHTML = initialValue;
   container.appendChild(span);
   parentContainer.appendChild(container);
}

export function setupCheckBoxControl(parentContainer, checkBoxId, labelText, initialValue)
{
   const container = createDiv();

   // label element
   const label = document.createElement('label');
   label.textContent = labelText;
   container.appendChild(label);

   // checkbox element
   const checkBox = document.createElement('input');
   checkBox.setAttribute('type', 'checkbox'); 
   checkBox.setAttribute('id', checkBoxId);
   checkBox.checked = initialValue;
   container.appendChild(checkBox);

   parentContainer.appendChild(container);
}

