# Communication API
Communication between the front end and backend is done using the TCP via local websockets. All messages sent are in the JSON format and are structured as defined by our API below.

## **Requests:** 
All requests are sent from the frontend to backend and use the same general structure that includes
  * “RequestId”: the key for connecting a response back to a request
  * “RequestType”: a key for identifying what is being requested 
  * “RequestData”: a json object containing relevant data for each specific request.

  <br>The following outlines all Requests built into our system.

* **HalfSizePreview:** requests the loading of a half sized preview of any number of images
    ```
    {
        "RequestID": <int>,
        "RequestType": "HalfSizePreview",
        "RequestData": {
            "names": ["<full file path img1>”, …]
        }
    }
    ```

* **Thumbnails:** requests the loading of any number of thumbnail images
    ```
    {
        "RequestID": <int>,
        "RequestType": "Thumbnails",
        "RequestData": {
            "names": ["<full file path img1>”, …] 
        }
    }
    ```

* **Process:** request the processing of input images
    ```
    {
        "RequestType": "Process",
        "RequestID": 1644011417091,
        "RequestData": {
        "images": [
            {"art":"<file path>", "white":"<file path>", "dark": "<file path>" },
            {"art":"<file path>", "white":"<file path>", "dark": "<file path>" }
            // An optional “target”:”<file path> can be included in both if the user chooses to include a target image separate from the art 
        ],
        “destinationDirectory”:"<path>",
        “sharpenString”: “N”,
        "targetLocation": {
            "top": 0.10977143435872826,
            "left": 0.15603501782610796,
            "bottom": 0.308251853284104,
            "right": 0.4105922998563207,
            "cols": 13,
            "rows": 10,
            “size”: 0.4,
            “whitePatch”: {“row”: 3, “col”: 5},
            "refData": {
                "name": “<full_path>",
                "standardObserver": 1931,
                "illuminants": "D50"
            },
        },
        // Optional “verificationLocation is only included if user includes a verification target
        "verificationLocation": {
            // Uses the same structure as the “targetLocation” above
        },
        
        }
    }
    ```
* **Reports:** request report information on results from a previously processed set of images
    ```
    {
        "RequestID": <int>​,
        "RequestType": "Reports",
        "RequestData": {
            “name”: <full path to project file>
        }
    }
    ```

* **ColorManagedImage:** request the loading of a full sized image created from a previously processed set of images
    ```
    {
        "RequestID": <int>​,
        "RequestType": "ColorManagedImage",
        "RequestData": {
            “name”: <full path to project file>
        }
    }
    ```

* **SpectralPicker:** request the estimated spectral reflectance of a group of selected pixels
    ```
    {
        "RequestID": <int>​,
        "RequestType": "SpectralPicker",
        "RequestData": {
            “name”: <full path to project file>
            “coordinates”: {“x”: <relative value>,“y”:  <relative value>},
            “size”: <relative size>
        }
    }
    ```

## Responses: 
All responces are sent from the backend to frontend and use the same general structure that includes

  * “RequestId”: the key for connecting the response back to a request
  * “ResponseType”: a key for identifying what type of response it is
  * “ResponseData”: a json object containing relevant data for each specific response.
  
The following outlines all Responses built into our system.

* **ImageBase64:** sends base64 representations of images
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "ImageBase64",
        "ResponseData": {
            "dataURL": "<base64 representation of image>",
        ​​	"name": "<key identifying the image>"
        }
    }
    ```
* **ImageBinary:** sends binary representations of images
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "ImageBinary",
        "ResponseData": {
            "type": "image/png" <MIME type>,
        ​​    "name": <key identifying the image>
        }
    }
    ```
    **NOTE:** This message only informs the frontend to expect binary data and does not acctualy include the binary data. The binary data is sent in a separately. The *public* `send_binary(btrgb::Image*, btrgb::image_quality)` method in `CommunicationsObj` handles this automaticly for the caller.

* **Progress:** send image processing progress as normalized percentage
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "Progress",
        "ResponseData": {
            "value": <0 to 1 inclusive>,
            "sender": <name of component sending progress>
        }
    }
    ```

* **Info:** send general information, primarily used for debugging
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "Info",
        "ResponseData": {
            "message": "<msg text>",
            "sender": "<name of sender>"
        }
    }
    ```

* **Error:** send error messages. Note: critical errors imply that the sending process have halted and cannot recover.
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "Error",
        "ResponseData": {
            "message": "<msg text> ",
            "sender": "<name of sender> ",
            “critical”:<boolean>

        }
    }
    ```

* **Process:** I am unsure of what(if any) this is being used for.
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "Process",
        "ResponseData": {
            "output": <output file path>
        }
    }
    ```

* **Reports:** send report processing results.
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "Reports",
        "ResponseData": {
            “reportType”: “Calibration”  //Allows for alternate for “Verification”
            “reports”: <json of report obj>
        }
    }
    ```

* **PipelineComponents:** send complete list of image processing components included in current execution of image processing pipeline
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "PipelineComponents",
        "ResponseData": {
            “components”: <json representation of component list>
        }
    }
    ```

* **PostCalirbation:** inform frontend that image processing is complete and where the results can be located.
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "CalibrationComplete",
        "ResponseData": {
            “path”: <results_path>
        }
    }
    ```

* **SpectralPicker:** send estimated spectral reflectance, in response to a SpectralPicker request
    ```
    {
        "RequestID": <int>​,
        "ResponseType": "SpectralPicker",
        "ResponseData": {
            “spectrum”: [36] <array of 36 float values between 0 and 1>
        }
    }
    ```
