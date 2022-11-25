<script lang="ts">
</script>

<main>
  <div class="title">About</div>
  <div class="body">
    <p>
      Beyond RGB is an open-source software application for processing spectral
      image sets. It takes as input two RAW (or unprocessed linear TIFF) RGB
      image sets (object/flatfield/darkfield) captured under two different
      lighting or filtering conditions, and outputs
    </p>
    <ol>
      <li>a color managed RGB image</li>
      <li>
        a spectral reflectance transform that enables the interactive estimation
        of reflectance curves from user-selected regions of interest in the
        image
      </li>
      <li>
        two sets of ancillary data and plots detailing the accuracy of the color
        and spectral calibrations, respectively
      </li>
    </ol>
    <br />
    <p>
      Beyond RGB was designed with considerations for form, function, and user
      friendliness, and is intended for use by cultural heritage imaging
      professionals. It is cross-platform compatible and operable via an
      interactive graphical user interface, and will continue to be a living,
      updatable open-source project. It is freely available for download from
      the RIT Munsell Color Science Laboratory website. Please direct comments
      and questions to susan.farnand at rit.edu.
    </p>
    <br />
    <p>
      The Quick Start Guide provides instructions on getting started with Beyond
      RGB. It gives an overview of the steps involved in
    </p>
    <ol>
      <li>
        setting up the software to import and process the spectral image stack
      </li>
      <li>
        inspecting and interacting with the output image and other files. There
        are also written cues describing the action to take on each screen
        within the application as it is navigated
      </li>
    </ol>
    <br />
    <b>Authored by</b>
    <p>
      John Akey,
      Elijah Cooper,
      Patrick Dalesio,
      Brendan Grau,
      Andrew Hammerstone,
      SungMin Kim,
      Tristan Knox,
      Keenan Miller,
      Jake O'Neil,
      and Jared Ponzetti.
    </p>
    <b>Sponsored by</b>
    <p>
      Susan Farnand,
      Leah Humenuck,
      and Olivia Kuzio.
    </p>
  </div>
</main>

<style lang="postcss">
  main {
    @apply bg-gray-600 rounded-xl overflow-auto;
  }
  .title {
    @apply text-2xl bg-gray-700 p-2;
  }
  .body {
    @apply p-4 flex flex-col gap-2 max-h-[70vh] overflow-auto select-text;
  }
  ol {
    list-style: decimal;
    list-style-position: outside;
    @apply bg-gray-700 pl-8 py-2 pr-2 rounded-xl;
  }
  li {
    @apply p-1;
  }
  p {
    @apply text-base;
  }

  /* width */
  ::-webkit-scrollbar {
    @apply relative transition-all w-2;
  }

  /* Track */
  ::-webkit-scrollbar-track {
    @apply bg-gray-800/30;
  }

  /* Handle */
  ::-webkit-scrollbar-thumb {
    @apply bg-gray-500 rounded-full;
  }

  /* Handle on hover */
  ::-webkit-scrollbar-thumb:hover {
    @apply bg-gray-500;
  }
</style>
