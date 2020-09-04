// Sliders and value boxes
let slidersAll = document.querySelectorAll(".slider");
let valueBoxAll = document.querySelectorAll(".value-box");

let shiftRightSlider = document.querySelector("#shift-right");
let shiftRightValueBox = document.querySelector("#shift-right-value-box");

let shiftDownSlider = document.querySelector("#shift-down");
let shiftdownValueBox = document.querySelector("#shift-down-value-box");

let blurSlider = document.querySelector("#blur");
let blurValueBox = document.querySelector("#blur-value-box");

let opacitySlider = document.querySelector("#opacity");
let opacityValueBox = document.querySelector("#opacity-value-box");

// Add events to sliders
for (let i = 0; i < slidersAll.length; i++) {
    slidersAll[i].addEventListener("change", function() {
        // if opacity function do something different:
        if (this === opacitySlider){
            resultingValue = this.value / 100;
            valueBoxAll[i].textContent = resultingValue;
        } else {
            valueBoxAll[i].textContent = slidersAll[i].value;
        }
        updatePreview();
        updateFirstShadow();
    });
}

// Color pickers
let shadowColor = document.querySelector("#shadow-color");
let textColor = document.querySelector("#text-color");
let backgroundColor = document.querySelector("#background-color");
// Container of preview text used to change background color
let previewContainer = document.querySelector("#preview-container");

// Add events to color pickers
shadowColor.addEventListener("change", function() {
    updatePreview();
    updateFirstShadow();
});

textColor.addEventListener("change", function() {previewText.style.color = this.value});
backgroundColor.addEventListener("change", function() {previewContainer.style.backgroundColor = this.value})

// Add new shadows
let addNewShadowButton = document.querySelector("#add-new-shadow");
addNewShadowButton.addEventListener("click", addNewShadow);
let savedShadowsList = document.querySelector("#saved-shadows-list");

// Update Preview Text variables
let lastRegex = new RegExp("rgb\\(206, 89, 55\\) -3px 4px 2px$", "g");
let previewText = document.querySelector("#preview-text");

function updatePreview() {
    /*Function responsible for updating preview text styling*/

    // Store current styling
    let updateStyleString = "window.getComputedStyle(previewText).textShadow";
    
    // If lacks styling 
    if (updateStyleString === "none" || updateStyleString === "") {
        updateStyleString = "";
        
        // Convert hexadecimal to rgba
        let rgba = hexToRGBA(shadowColor.value, opacityValueBox.textContent);
        updateStyleString += rgba;
        
        // Get vlaues from each box and add them to sring
        for (let i = 0; i < valueBoxAll.length; i++){
            if (valueBoxAll[i] !== opacityValueBox) {
                updateStyleString += valueBoxAll[i].textContent + "px ";
            }
        }
        previewText.style.textShadow = updateStyleString; 
    } else {
        // Else when editing current shadow
        
        // String to replace old style and string to repalce old regex
        let newStyle = "";
        let newRegex = "";
        
        // Convert hex to RGBA
        let rgba = hexToRGBA(shadowColor.value, opacityValueBox.textContent);
        let newRegexColor = rgbaToRegExp(rgba);
        
        // Add space before adding shifts and blur
        newStyle += rgba + " ";
        newRegex += newRegexColor + " ";
        
        // Get vlaues from each box and add them to srings
        for (let i = 0; i < valueBoxAll.length; i++){
            if (valueBoxAll[i] !== opacityValueBox) {
                newStyle += valueBoxAll[i].textContent + "px ";
                newRegex += valueBoxAll[i].textContent + "px ";
            }
        }
        
        // Add $ to regex so it only replaces last shadow in styling
        newRegex = newRegex.trim() + "$";
        previewText.style.textShadow = previewText.style.textShadow.replace(lastRegex, newStyle);
        
        // Update regex to match the new style
        lastRegex = new RegExp(newRegex, "g");
    }
    updateCodeBlock();
}

function hexToRGBA(colorValue, opacity) {
    // Funciton resposible for converting hexadecimal to rgba color
    let value = colorValue.match(/[A-Za-z0-9]{2}/g);
    value = value.map(function(v) {
        return parseInt(v, 16);
    });
    if (opacity === '1' || opacity === '1.00' || opacity === '1.0') {
        // When opacity is equal to 1 then return rgb because html automatically converts it to such
        return "rgb(" + value.join(", ") + ")"
    } else {
        return "rgba(" + value.join(", ") + ", " + opacity + ")";
    }    
}

function rgbaToRegExp(rgba) {
    /*Function responsible for formating rgb to fit regex --> adding escape characters*/
    let escapeParenthesesLeft = /\(/g;
    let escapeParenthesesRight = /\)/g;
    let escapePeriod = /\./g;
    rgba = rgba.replace(escapeParenthesesLeft, "\\(");
    rgba = rgba.replace(escapeParenthesesRight, "\\)");
    rgba = rgba.replace(escapePeriod, "\\.");
    return rgba;
}

function updateFirstShadow() {
    /*Function responsible for updating changes made to first shadow*/
    let firstShadow = document.querySelectorAll(".saved-shadow-container")[0];
    let getStyle = window.getComputedStyle(previewText).textShadow;
    let textToDisplay = getStyle.match(lastRegex)[0];
    firstShadow.childNodes[0].nodeValue = textToDisplay;
    firstShadow.style.textShadow = textToDisplay;
}

function addNewShadow() {
    /*Function responsible for adding new shadows to list and upgrading style of previewText*/
    
    // Get first shadow
    let allShadowsAvailable = document.querySelectorAll(".saved-shadow-container");
    let shadowToClone = allShadowsAvailable[0];
    
    // Clone shadow
    let clonedShadow = shadowToClone.cloneNode(true);
    let indexNumber = allShadowsAvailable.length;
    
    // Add option to delete it when clicked on button
    clonedShadow.children[0].onclick = function() {deleteShadow(indexNumber)};

    // Get style
    let getStyle = shadowToClone.style.textShadow;

    // Append new shadow to list
    savedShadowsList.appendChild(clonedShadow);

    // Append new shadow to preview text
    previewText.style.textShadow = getStyle + ", " + previewText.style.textShadow;
    updateCodeBlock();
}

function deleteShadow(indexNumber) {
    /*Function responsible for deleting shadows.*/
    
    let allShadows = document.querySelectorAll(".saved-shadow-container");
    let shadowToRemove = allShadows[indexNumber];
    
    // Remove styling from preview text
    removeStyling(shadowToRemove.style.textShadow);
    
    // Remove shadow
    shadowToRemove.remove();

    // New list because deleted some event
    let allShadowsNew = document.querySelectorAll(".saved-shadow-container");
    
    // Upgrade onclick event
    for (let i = 0; i < allShadowsNew.length; i++) {
        allShadowsNew[i].children[0].onclick = function() {deleteShadow(i)};
    }
}

function removeStyling(shadowStyle) {
    /*Function responsible for removing styling from preview text after deleting one of shadow options*/

    // Format shadow style to fit regex
    shadowStyle = "\\s?" + shadowStyle;
    shadowStyle += ",?";
    shadowStyleRegex = new RegExp(rgbaToRegExp(shadowStyle));
    
    // Fetch whole style
    let updateStyleString = window.getComputedStyle(previewText).textShadow;
    updateStyleString = updateStyleString.replace(shadowStyleRegex, "");
    
    // Update previewText's style
    previewText.style.textShadow = updateStyleString;
    updateCodeBlock();
}

function updateCodeBlock() {
    let codeBlock = document.querySelector("#code-block");
    let previewStyle = window.getComputedStyle(previewText).textShadow;
    let content = "text-shadow: ";
    content += previewStyle + ";";

    codeBlock.innerText = content

}