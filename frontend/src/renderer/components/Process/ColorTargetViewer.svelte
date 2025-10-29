<script lang="ts">
    import { currentPage, processState } from "@util/stores";

    import OpenSeadragon from "openseadragon";
    import { afterUpdate, onDestroy, onMount } from "svelte";
    import { getZoomPercentage } from "@util/photoViewerHelper";
    import ca from "date-fns/locale/ca";

    let viewer;
    let mouseTracker;
    let colorOverlay;
    let verifyOverlay;

    let trackers = [];

    let pressPos = { top: 0, bottom: 0, left: 0, right: 0 };
    let viewportPoint;
    let imagePoint;

    export let viewerOpen = false;
    export let colorTarget;
    export let verifyTarget;

    export let colorPos;
    export let verifyPos;

    export let loading;

    export let linearZoom = 100;

    export let calibrationTargetRotationAngle = 0; // Rotation angle in degrees

    let imageUrl;

    const createViewer = () => {
        viewer = OpenSeadragon({
            id: "color-seadragon-viewer",
            prefixUrl: "/openseadragon/images/",
            immediateRender: true,
            preload: true,
            showNavigator: false,
            minZoomLevel: 1,
            useCanvas: true,
            showZoomControl: false,
            showHomeControl: false,
            showFullPageControl: false,
            preserveImageSizeOnResize: false,
            maxZoomPixelRatio: 30,
            zoomPerScroll: 1.15,
            visibilityRatio: 1,
            animationTime: 0.4,
        });
        viewer.addHandler("zoom", (e) => setTimeout(() => handleZoom(e), 100));
        viewer.addHandler("open", () => (viewerOpen = true));
    };

    const destroyViewer = () => {
        if (viewer) {
            viewer.destroy();
            viewer = null;
            viewerOpen = false;
            console.log("Color target viewer destroyed");
        }
    };

    onMount(() => {
        console.log("Color target viewer Mount");
        createViewer();
    });
    onDestroy(() => {
        console.log("Color target viewer Destroy");
        destroyViewer();
    });

    function handleZoom(e) {
        if (e.zoom > 10) {
            let drawer = viewer.drawer;
            drawer.setImageSmoothingEnabled(false);
        } else {
            let drawer = viewer.drawer;
            drawer.setImageSmoothingEnabled(true);
        }

        linearZoom = getZoomPercentage(viewer.viewport.getZoom(true));
    }

    $: if ($processState.currentTab === 5) {
        if (viewer && !viewer.isOpen()) {
            let temp = new Image();

            temp.src = $processState.artStacks[0].colorTargetImage?.dataURL;
            imageUrl = temp.src;

            console.log("Opening Image");
            console.log({ IMAGEURL: imageUrl });
            console.log({ INCLUDES: imageUrl.includes("blob") });
            if (imageUrl.includes("blob")) {
                loading = false;
            }

            viewer.open({
                type: "image",
                url: imageUrl,
            });
        }
    }

    // Close the viewer once processing starts
    $: if ($processState.currentTab === 6) {
        viewer.close();
        viewerOpen = false;
    }

    $: if (viewer) {
        console.log(viewer.isOpen());
    }

    function removeOverlay(id) {
        console.log("Entering removeOverlay");
        const selBox = document.getElementById(`sBox-${id}`);
        const gridBox = document.getElementById(`gBox-${id}`);

        viewer.removeOverlay(selBox);
        if (id === 0) {
            colorOverlay = false;
        } else if (id === 1) {
            verifyOverlay = false;
        }
        trackers[id].destroy();
    }

    function addOverlay(id) {
        console.log("Entering addOverlay");
        const selBox = document.getElementById(`sBox-${id}`);
        const gridBox = document.getElementById(`gBox-${id}`);
        if (viewer && selBox && gridBox) {
            if (id === 0) {
                gridBox.style.gridTemplateColumns = `repeat(${colorTarget.cols}, auto)`;
            } else if (id === 1) {
                gridBox.style.gridTemplateColumns = `repeat(${verifyTarget.cols}, auto)`;
            }
            console.log(`Adding Overlay ${id}`);

            if (id === 0) {
                viewer.addOverlay({
                    element: selBox,
                    location: new OpenSeadragon.Rect(
                        colorPos.left,
                        colorPos.top,
                        colorPos.right - colorPos.left,
                        colorPos.bottom - colorPos.top
                    ),
                });
            } else if (id === 1) {
                viewer.addOverlay({
                    element: selBox,
                    location: new OpenSeadragon.Rect(
                        verifyPos.left,
                        verifyPos.top,
                        verifyPos.right - verifyPos.left,
                        verifyPos.bottom - verifyPos.top
                    ),
                });
            }

            if (id === 0) {
                colorOverlay = true;
            } else if (id === 1) {
                verifyOverlay = true;
            }

            trackers[id] = new OpenSeadragon.MouseTracker({
                element: `sBox-${id}`,
                pressHandler: function (e) {
                    var overlay = viewer.getOverlayById(`sBox-${id}`);

                    // Overlay box coords
                    let topPos = overlay.bounds.y;
                    let botPos = overlay.bounds.y + overlay.bounds.height;
                    let leftPos = overlay.bounds.x;
                    let rightPos = overlay.bounds.x + overlay.bounds.width;

                    // Event Relative coords
                    let offset = viewer.viewport.deltaPointsFromPixels(
                        e.position
                    );
                    let eViewX = leftPos + offset.x;
                    let eViewY = topPos + offset.y;

                    // Event Distance to Edge
                    pressPos = {
                        ele: e.originalEvent.target,
                        top: Math.abs(eViewY - topPos),
                        left: Math.abs(eViewX - leftPos),
                        right: Math.abs(eViewX - rightPos),
                        bottom: Math.abs(eViewY - botPos),
                    };
                    e.originalEvent.path.forEach((element) => {
                        if (element.classList?.length > 0) {
                            if (element.classList[0] === "inc") {
                                console.log("increase");
                                if (
                                    element.parentElement.classList[1] ===
                                        "top" ||
                                    element.parentElement.classList[1] ===
                                        "bottom"
                                ) {
                                    if (id === 0) {
                                        colorTarget.rows = colorTarget.rows + 1;
                                    } else if (id === 1) {
                                        verifyTarget.rows =
                                            verifyTarget.rows + 1;
                                    }
                                } else if (
                                    element.parentElement.classList[1] ===
                                        "left" ||
                                    element.parentElement.classList[1] ===
                                        "right"
                                ) {
                                    if (id === 0) {
                                        colorTarget.cols = colorTarget.cols + 1;
                                    } else if (id === 1) {
                                        verifyTarget.cols =
                                            verifyTarget.cols + 1;
                                    }
                                }
                            } else if (element.classList[0] === "dec") {
                                console.log("decrease");
                                if (
                                    element.parentElement.classList[1] ===
                                        "top" ||
                                    element.parentElement.classList[1] ===
                                        "bottom"
                                ) {
                                    if (id === 0) {
                                        colorTarget.rows = colorTarget.rows - 1;
                                    } else if (id === 1) {
                                        verifyTarget.rows =
                                            verifyTarget.rows - 1;
                                    }
                                } else if (
                                    element.parentElement.classList[1] ===
                                        "left" ||
                                    element.parentElement.classList[1] ===
                                        "right"
                                ) {
                                    if (id === 0) {
                                        colorTarget.cols = colorTarget.cols - 1;
                                    } else if (id === 1) {
                                        verifyTarget.cols =
                                            verifyTarget.cols - 1;
                                    }
                                }
                            }
                        }
                    });
                },
                dragHandler: function (e) {
                    var overlay = viewer.getOverlayById(`sBox-${id}`);

                    let viewDeltaPoint = viewer.viewport.deltaPointsFromPixels(
                        e.delta
                    );

                    // https://openseadragon.github.io/docs/OpenSeadragon.Rect.html
                    // x and y is top left corner. width to the right and height downwards
                    let box = new OpenSeadragon.Rect(
                        overlay.bounds.x,
                        overlay.bounds.y,
                        overlay.width,
                        overlay.height
                    );

                    // Beginning of the movement/rotation handling math.
                    // There is a diagram in the GitHub docs folder for your viewing pleasure that
                    // visualizes the math going on here. Hopefully it helps and is not just
                    // heiroglyphics.

                    const rotatePoints = (x, y, degrees) => {
                        const rad = degrees * (Math.PI / 180);
                        const cos = Math.cos(rad);
                        const sin = Math.sin(rad);
                        return [x * cos - y * sin, x * sin + y * cos];
                    };

                    const getBoxCenterOfRotation = (box) => {
                        return [box.x + box.width / 2, box.y + box.height / 2];
                    };

                    let dx = viewDeltaPoint.x;
                    let dy = viewDeltaPoint.y;
                    if (calibrationTargetRotationAngle != 0) {
                        [dx, dy] = rotatePoints(
                            viewDeltaPoint.x,
                            viewDeltaPoint.y,

                            // negative b/c we are unrotating the mouse movement
                            // to align with the unrotated version of the overlay.
                            -calibrationTargetRotationAngle
                        );
                    }

                    // This numbered list of steps walks through the process of compensating for the fact that
                    // the center of rotation of the box moves with the box when dragged. Because of this, we have
                    // to ensure the box is moved such that its center of rotation follows the rotated overlay's center of rotation,
                    // not the original box's center of rotation.

                    // 1. Get the coordinates of the center of rotation (CoR) of the box before being dragged.
                    const [cor_prev_x, cor_prev_y] =
                        getBoxCenterOfRotation(box);

                    if (!pressPos.ele.classList.contains("corner")) {
                        // Dragging the entire target: axis‐aligned move
                        // Means the user is dragging on the target itself, not the corners.
                        box.x += viewDeltaPoint.x;
                        box.y += viewDeltaPoint.y;
                    } else {
                        // 2. Edit the box properties such that it is moved to its newly dragged location.
                        if (pressPos.ele.classList[1] === "tl") {
                            box.y += dy;
                            box.height -= dy;
                            box.x += dx;
                            box.width -= dx;
                        } else if (pressPos.ele.classList[1] === "tr") {
                            box.y += dy;
                            box.height -= dy;
                            box.width += dx;
                        } else if (pressPos.ele.classList[1] === "bl") {
                            box.height += dy;
                            box.x += dx;
                            box.width -= dx;
                        } else if (pressPos.ele.classList[1] === "br") {
                            box.height += dy;
                            box.width += dx;
                        }

                        // 3. Get the coordinates of the CoR of the box after being dragged.
                        const [cor_next_x, cor_next_y] =
                            getBoxCenterOfRotation(box);

                        // 4. Calculate the change in the CoR (dCoR) of the box from the drag.
                        const d_cor_x = cor_next_x - cor_prev_x;
                        const d_cor_y = cor_next_y - cor_prev_y;

                        // 5. Reset the box to its previous location.
                        box.x -= d_cor_x;
                        box.y -= d_cor_y;

                        // 6. Rotate dCoR based on the calibration target's rotation angle.
                        const [d_cor_r_x, d_cor_r_y] = rotatePoints(
                            d_cor_x,
                            d_cor_y,
                            calibrationTargetRotationAngle
                        );

                        // 7. Move the box properly such that its CoR is aligned with the rotation.
                        box.x += d_cor_r_x;
                        box.y += d_cor_r_y;
                    }

                    overlay.update(box);
                    overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);

                    // Re-apply the visual rotation
                    const selectorBox = document.getElementById(`sBox-${id}`);
                    if (selectorBox) {
                        selectorBox.style.transform = `rotate(${calibrationTargetRotationAngle}deg)`;
                        selectorBox.style.transformOrigin = "center";
                    }

                    // Update position
                    if (id === 0) {
                        colorPos = {
                            top: box.y,
                            left: box.x,
                            bottom: box.y + box.height,
                            right: box.x + box.width,
                        };
                    } else if (id === 1) {
                        verifyPos = {
                            top: box.y,
                            left: box.x,
                            bottom: box.y + box.height,
                            right: box.x + box.width,
                        };
                    }
                },
            });
        }
    }

    let root = document.documentElement;

    // ----------------------------------------
    $: if (colorTarget && !colorOverlay && viewerOpen) {
        console.log("Color Target");
        setTimeout(() => {
            addOverlay(0);
            setTimeout(() => {
                colorTarget.color = colorTarget.color + 1;
            }, 0);
        }, 0);
    }

    $: if (!colorTarget && colorOverlay) {
        console.log("Color Target REMOVE");

        removeOverlay(0);
    }

    $: if (colorTarget?.color) {
        root.style.setProperty("--color_hue", `${colorTarget.color}`);
    }

    $: if (colorTarget?.rows) {
        const gridBox = document.getElementById("gBox-0");
        if (gridBox) {
            gridBox.style.gridTemplateRows = `repeat(${colorTarget.rows}, auto)`;
        }
    }
    $: if (colorTarget?.cols) {
        const gridBox = document.getElementById("gBox-0");
        if (gridBox) {
            gridBox.style.gridTemplateColumns = `repeat(${colorTarget.cols}, auto)`;
        }
    }

    $: if (colorTarget?.size) {
        root.style.setProperty("--color_size", `${colorTarget.size * 100}%`);
    }

    // --------------------------------------
    $: if (verifyTarget && !verifyOverlay && viewerOpen) {
        console.log("Verify Target");
        setTimeout(() => {
            addOverlay(1);
            setTimeout(() => {
                verifyTarget.color = verifyTarget.color + 1;
            }, 0);
        }, 0);
    }

    $: if (!verifyTarget && verifyOverlay) {
        console.log("Verify Target REMOVE");

        removeOverlay(1);
    }

    $: if (verifyTarget?.color) {
        console.log("verify hue");
        root.style.setProperty("--verfiy_hue", `${verifyTarget.color}`);
    }

    $: if (verifyTarget?.size) {
        console.log("verify size");

        root.style.setProperty("--verify_size", `${verifyTarget.size * 100}%`);
    }

    $: if (verifyTarget?.rows) {
        const gridBox = document.getElementById("gBox-1");
        if (gridBox) {
            gridBox.style.gridTemplateRows = `repeat(${verifyTarget.rows}, auto)`;
        }
    }

    $: if (verifyTarget?.cols) {
        const gridBox = document.getElementById("gBox-1");
        if (gridBox) {
            gridBox.style.gridTemplateColumns = `repeat(${verifyTarget.cols}, auto)`;
        }
    }

    //Function to be ran after changes are made to the coordinate inputs in ColorTarget.svelte
    //Checks the new coordinate values and updates to box accordingly
    export function updateCoords() {
        let overlay = viewer.getOverlayById(`sBox-0`);

        let box = new OpenSeadragon.Rect(
            colorPos.left,
            colorPos.top,
            colorPos.right - colorPos.left,
            colorPos.bottom - colorPos.top
        );

        overlay.update(box);
        overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);

        // Re-apply the CSS rotation so it stays at calibrationTargetRotationAngle
        const selectorBox = document.getElementById("sBox-0");
        if (selectorBox) {
            selectorBox.style.transform = `rotate(${calibrationTargetRotationAngle}deg)`;
            selectorBox.style.transformOrigin = "center";
        }
    }

    //Same as updateCoords() for the VerifyTarget
    export function updateVerifyCoords() {
        let overlay = viewer.getOverlayById(`sBox-1`);

        let box = new OpenSeadragon.Rect(
            verifyPos.left,
            verifyPos.top,
            verifyPos.right - verifyPos.left,
            verifyPos.bottom - verifyPos.top
        );

        overlay.update(box);
        overlay.drawHTML(viewer.overlaysContainer, viewer.viewport);
    }

    export function getResolution() {
        var coordpoint = viewer.viewport.viewportToImageCoordinates(
            new OpenSeadragon.Point(1, 0)
        );
        return coordpoint.x;
    }
</script>

<main>
    <div id="color-seadragon-viewer" />
    <!-- only show the zoom percentage if it is greater than 1% -->
    {#if linearZoom > 1}
        <h1 id="zoom">{Math.floor(linearZoom)}%</h1>
    {/if}

    {#each [colorTarget, verifyTarget] as target, i}
        {#if target}
            <div
                class="selectorBox"
                class:ver={i === 1}
                id={`sBox-${i}`}
                style="transform: rotate({calibrationTargetRotationAngle}deg); transform-origin: center;"
            >
                <div class="layout">
                    <div class="gridBox" id={`gBox-${i}`}>
                        {#each [...Array(target.rows * target.cols).keys()].map((i) => i + 1) as boxIndex}
                            <div
                                class="line"
                                class:ver={i === 1}
                                class:whitePoint={i !== 1 &&
                                    target.whitePatch.row != null &&
                                    target.whitePatch.col != null &&
                                    target.whitePatch.row +
                                        (target.whitePatch.col - 1) *
                                            target.rows ===
                                        boxIndex}
                            >
                                <div
                                    class:target={i === 0}
                                    class:verTarget={i === 1}
                                    class:whitePoint={i !== 1 &&
                                        target.whitePatch.row != null &&
                                        target.whitePatch.col != null &&
                                        target.whitePatch.row +
                                            (target.whitePatch.col - 1) *
                                                target.rows ===
                                            boxIndex}
                                />
                                <!-- <span class="targetNum"><svg><text>{boxIndex}</text></svg></span> -->
                            </div>
                        {/each}
                    </div>
                </div>
                <div class="corner tl" />
                <div class="corner tr" />
                <div class="corner bl" />
                <div class="corner br" />
            </div>
        {/if}
    {/each}
</main>

<style lang="postcss">
    :root {
        --color_hue: 50;
        --verfiy_hue: 100;
        --verify_size: 50%;
        --color_size: 50%;
    }

    button {
        @apply ring-0;
    }

    main {
        @apply w-full h-[90%] ring-1 ring-gray-800 bg-gray-900/50 aspect-[3/2] shadow-lg flex flex-col;
    }
    #color-seadragon-viewer {
        @apply h-full w-full;
    }
    .selectorBox {
        /* background-size: 40px 40px;
    background-image: linear-gradient(to right, black 2px, transparent 1px),
      linear-gradient(to bottom, black 2px, transparent 1px),
      radial-gradient(circle, #000000 3px, rgba(0, 0, 0, 0) 1px); */

        @apply bg-transparent relative filter;
    }
    .gridBox {
        display: grid;
        grid-template-rows: auto;
        grid-auto-flow: column;
        grid-area: box;
        @apply w-full h-full overflow-hidden z-40;
    }
    .line {
        background: radial-gradient(
            circle at center,
            transparent 80%,
            hsla(var(--color_hue), 100%, 50%, 0.3) 80%
        );
        border: solid;
        border-color: hsl(var(--color_hue), 100%, 50%);

        @apply w-full h-full border-[2px] flex items-center justify-center relative overflow-hidden z-10;
    }

    .ver {
        border-color: hsl(var(--verfiy_hue), 100%, 50%);
    }

    .ver .line {
        background: radial-gradient(
            circle at center,
            transparent 80%,
            hsla(var(--verfiy_hue), 100%, 50%, 0.3) 80%
        );
    }

    .layout {
        display: grid;
        grid-template-rows: 14% auto 14%;
        grid-template-columns: 14% auto 14%;
        /* grid-auto-flow: column; */
        grid-template-areas:
            "top top top"
            "left box right"
            "bottom bottom bottom";
        @apply w-[140%] h-[140%] absolute -top-[20%] -left-[20%] overflow-hidden;
    }
    .exp {
        @apply bg-transparent hover:bg-green-400/50 border-2 
          border-gray-600/25 hover:border-green-500 z-0 w-full h-full
          duration-500 transition-all delay-150 ease-in flex justify-center rounded-lg
          gap-1;
    }
    .inc {
        @apply bg-transparent text-transparent group-hover:text-white group-hover:bg-green-500/90
            align-middle justify-center flex items-center transition-all delay-200;
    }
    /* w-[40%] h-[80%] */
    .dec {
        @apply bg-transparent text-transparent group-hover:text-white group-hover:bg-red-500/90
            align-middle justify-center flex items-center transition-all delay-200;
    }
    .top {
        grid-area: top;
        @apply w-[70%] ml-[15%] flex overflow-hidden;
    }
    .top .inc {
        @apply w-1/4;
    }
    .top .dec {
        @apply w-1/4;
    }
    .left {
        grid-area: left;
        @apply flex flex-col-reverse overflow-hidden;
    }
    .left .inc {
        @apply h-1/4;
    }
    .left .dec {
        @apply h-1/4;
    }
    .right {
        grid-area: right;
        @apply flex flex-col-reverse overflow-hidden;
    }
    .right .inc {
        @apply h-1/4;
    }
    .right .dec {
        @apply h-1/4;
    }
    .bottom {
        grid-area: bottom;
        @apply w-[70%] ml-[15%] flex overflow-hidden;
    }
    .bottom .inc {
        @apply w-1/4;
    }
    .bottom .dec {
        @apply w-1/4;
    }
    .target {
        border: solid;
        border-color: hsla(var(--color_hue), 100%, 50%, 0.5);
        width: var(--color_size);
        height: var(--color_size);
        @apply border-[3px] rounded-none;
    }

    .verTarget {
        border: solid;
        border-color: hsla(var(--verfiy_hue), 100%, 50%, 0.5);
        width: var(--verify_size);
        height: var(--verify_size);
        @apply border-[3px] rounded-none;
    }
    .line.whitePoint {
        background: radial-gradient(
            circle at center,
            transparent 80%,
            hsla(0, 0%, 100%, 1) 80%
        );
        border-color: hsla(0, 0%, 100%, 1);
    }
    .line > .target.whitePoint {
        border: dashed;
        border-color: hsla(0, 0%, 100%, 1);
        @apply border-[3px] rounded-none;
    }

    .corner {
        background-color: hsla(var(--color_hue), 100%, 50%, 0.75);
        @apply absolute h-[5%] aspect-square z-50;
    }

    .ver .corner {
        background-color: hsla(var(--verfiy_hue), 100%, 50%, 0.75);
    }

    .tl {
        @apply -top-[2.5%] -left-[2%] cursor-nw-resize;
    }
    .tr {
        @apply -top-[2.5%] -right-[2%] cursor-ne-resize;
    }
    .bl {
        @apply -bottom-[2.5%] -left-[2%] cursor-ne-resize;
    }
    .br {
        @apply -bottom-[2.5%] -right-[2%] cursor-nw-resize;
    }
    #rowCol {
        @apply bg-transparent flex;
    }
    .targetNum {
        color: hsla(var(--color_hue), 100%, 50%, 0.75);
        @apply absolute top-0 left-0 p-1 text-base font-semibold;
    }

    #rowCol > p {
        @apply bg-black flex px-2;
    }
    /* width */
    ::-webkit-scrollbar {
        @apply relative transition-all w-2 h-2;
    }

    /* Track */
    ::-webkit-scrollbar-track {
        @apply bg-transparent;
    }

    /* Handle */
    ::-webkit-scrollbar-thumb {
        @apply bg-red-500 rounded-full;
    }

    /* Handle on hover */
    ::-webkit-scrollbar-thumb:hover {
        @apply bg-red-500;
    }
</style>
