const descriptionEl = document.querySelector('#description')
const descriptionStart = 'I am a... '
const descriptionItems = [
    'Web Designer',
    'Software Developer',
    'Mobile Developer',
    '.NET Developer'
]
const pauseIntervalSec = 3
const setIntervalTimeMilSec = 100

let startCharCounter = 0
let itemsCharCounter = 0
let itemsWordCounter = 0 
let writeDescriptionStart = true
let pauseWriting = false
let stopWritingTimerMilSec = pauseIntervalSec * setIntervalTimeMilSec * 10
let stopWritingCountdown = stopWritingTimerMilSec

descriptionEl.textContent = ''

setInterval(function(){
    if(pauseWriting){
        stopWritingCountdown -= setIntervalTimeMilSec
        if(stopWritingCountdown <= 0){
            pauseWriting = false
            stopWritingCountdown = stopWritingTimerMilSec
            if(writeDescriptionStart){
                descriptionEl.textContent = ''
            } else {
                descriptionEl.textContent = descriptionStart
            }
            
        }
    } else if(writeDescriptionStart){
        descriptionEl.textContent += descriptionStart[startCharCounter]
        startCharCounter++
        if(startCharCounter >= descriptionStart.length){
            writeDescriptionStart = false
            startCharCounter = 0 
        }
    } else{
        descriptionEl.textContent += descriptionItems[itemsWordCounter][itemsCharCounter]
        itemsCharCounter++
        if(itemsCharCounter >= descriptionItems[itemsWordCounter].length){
            if(itemsWordCounter >= descriptionItems.length - 1){
                itemsWordCounter = 0
                writeDescriptionStart = true 
            } else {
                itemsWordCounter++       
            }   
            itemsCharCounter = 0
            pauseWriting = true            
        }
    }
}, setIntervalTimeMilSec)
