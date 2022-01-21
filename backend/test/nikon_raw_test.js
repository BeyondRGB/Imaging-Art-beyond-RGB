/*
 * Requires the nikon raw test files to be in the same
 * directory as the backend executable.
 * 
 * Run with: 
node nikon_raw_test.js
 */
const WebSocket = require('ws');

w = new WebSocket("ws://localhost:9002");

w.onmessage = (e) => {
    console.log(e.data);
    if(e.data.includes("Image Processing Done!!!")) {
        w.close();
        process.exit();
    }
}

w.onopen = () => {
    req = {
        "RequestType": "processImg",
        "RequestData": {
            "Images":[
                {
                    "Art": "nikon_targets_1.NEF",
                    "White": "nikon_white_1.NEF",
                    "Dark": "nikon_dark_1.NEF"
                },
                {
                    "Art": "nikon_targets_2.NEF",
                    "White": "nikon_white_2.NEF",
                    "Dark": "nikon_dark_2.NEF"
                }
            ],
            "TargetLocation":{
                "TopLeft":{"x": 0, "y":0},
                "TopRight": {"x":50, "y":0},
                "BottomRight": {"x":50, "y":50},
                "BottomLeft": {"x":0, "y":50},
                "NumRows": 10,
                "NumCols":10
            }	
                
        }
    };
    w.send(JSON.stringify(req));
};


process.stdin.on('data', e => {
    w.close();
    process.exit();
});