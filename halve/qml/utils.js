
function showMessage(parent, msg, btn) {
    var component = Qt.createComponent("MessageTip.qml");
    if (component.status === Component.Ready) {
        var dialogObject = component.createObject(parent, {text:msg, standardButtons:btn?btn:Dialog.Yes});
        dialogObject.closed.connect(function() {
            dialogObject.destroy()
        });
        dialogObject.x = (parent.width - dialogObject.width) / 2;
        dialogObject.y = (parent.height - dialogObject.height) / 2;
        dialogObject.open();
    } else {
        console.log("Component not ready");
    }
}

function showTip(parent, msg) {
    var component = Qt.createComponent("MessageTip2.qml");
    if (component.status === Component.Ready) {
        var dialogObject = component.createObject(parent);
        dialogObject.closed.connect(function() {
            dialogObject.destroy()
        });
        dialogObject.show(msg);
    } else {
        console.log("Component not ready");
    }
}

var SurveyorRegulateList = [1,1.25,2.5,5,10,15,20,25,30,35,40];

function calculateDistance(x1, y1, x2, y2) {
    var dx = x2 - x1;
    var dy = y2 - y1;
    return Math.sqrt(dx * dx + dy * dy);
}

function getNotebookItemIcon(type, genreId) {
    switch(type) {
    case Halve.GENRE: {
        switch(genreId) {
        case 0: {
            return "qrc:/assets/images/event.svg";
        }
        case 1: {
            return  "qrc:/assets/images/snapshot.svg";
        }
        case 2: {
            return  "qrc:/assets/images/snapping.svg";
        }
        case 3: {
            return  "qrc:/assets/images/picsave.png";
        }
        case 4: {
            return  "qrc:/assets/images/mapping.png";
        }
        case 5: {
            return "qrc:/assets/images/videosave.png";
        }
        }
        break;
    }
    case Halve.SNAPSHOT: {
        return  "qrc:/assets/images/snapping.svg";
    }
    case Halve.AUTORECORD: {
        return  "qrc:/assets/images/video.png";

    }
    case Halve.MAPPING: {
        return  "qrc:/assets/images/group.svg";
    }
    case Halve.SCREENSHOT: {
        return  "qrc:/assets/images/picsave.png";

    }
    case Halve.SCREENRECORD: {
        return  "qrc:/assets/images/video.png";
    }
    }
}
