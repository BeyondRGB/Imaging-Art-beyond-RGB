<script lang="ts">
	import { FilePlusIcon } from "svelte-feather-icons";
	import { processState, sendMessage, setTabCompleted } from "@util/stores";
	import { forEach, find } from "lodash";
	import { countFields } from "@root/util/storesUtil";
	import ImageBubble from "@components/Process/ImageBubble.svelte";
	import Button from "@components/Button.svelte";
	import Dropzone from "svelte-file-dropzone";
	export let label = "Select Files";
	export let icon = FilePlusIcon;
	export let largeText = false;
	export let removeButton = false;
	let filePaths = [];
	let files = {
		accepted: [],
		rejected: [],
	};

	function getThumbnails() {
		const thumbnailID = Math.floor(Math.random() * 999999999);
		processState.update(state => ({
			...state,
			thumbnailID: thumbnailID,
		}));
		let msg = {
			RequestID: thumbnailID,
			RequestType: "Thumbnails",
			RequestData: {
				names: filePaths,
			},
		};
		sendMessage(JSON.stringify(msg));
		filePaths = [];
	}

	$: if ($processState.imageFilePaths.length >= 6 && !$processState.completedTabs[1]) {
		setTabCompleted(1);
	}

	function handleFilesSelect(e) {
		files.accepted = [];
		if (e.detail == null) {
			/* handles files being added from the select files button */
			files.accepted = [...files.accepted, ...e.target.files];
			// Reset input value to allow re-adding the same files
			e.target.value = "";
		} else {
			/* handles files being added from dragging and dropping */
			files.accepted = [...e.detail?.acceptedFiles];
		}

		const newFiles = [];
		forEach(files.accepted, f => {
			if (!find($processState.imageFilePaths, { id: f.path, name: f.name })) {
				newFiles.push({
					id: f.path,
					name: f.path,
				});
			}
		});
		processState.update(state => {
			const updatedFilePaths = [...state.imageFilePaths, ...newFiles];
			const totalImageCount = updatedFilePaths.length + countFields(state.artStacks[0].fields);
			let artImageCount = state.artImageCount;
			if (totalImageCount >= 6 && totalImageCount <= 8) {
				artImageCount = 1;
			} else if (totalImageCount > 8) {
				artImageCount = Math.ceil((totalImageCount - 6) / 2);
			}
			// Update filePaths array for thumbnail request
			forEach(updatedFilePaths, function (f) {
				filePaths.push(f.name);
			});
			return {
				...state,
				imageFilePaths: updatedFilePaths,
				artImageCount: artImageCount,
			};
		});
		getThumbnails();
	}

	const remove = item => {
		processState.update(state => ({
			...state,
			imageFilePaths: state.imageFilePaths.filter(value => value.id !== item.id),
		}));
	};

	const removeAll = () => {
		processState.update(state => ({
			...state,
			imageFilePaths: [],
		}));
	};

	function openAttachment() {
		document.getElementById("attachment").click();
	}
</script>

<main>
	<br />
	<Dropzone
		on:drop={handleFilesSelect}
		noClick
		disableDefaultStyles
		containerClasses="custom-dropzone"
	>
		<input
			type="file"
			class="file"
			id="attachment"
			style="display: none;"
			on:change={handleFilesSelect}
			multiple
		/>
		{#if $processState.imageFilePaths?.length > 0}
			<div class="button-row">
				<Button
					variant="default"
					size={largeText ? "lg" : "md"}
					onClick={openAttachment}
					{icon}
					iconPosition="right"
				>
					{label}
				</Button>
				<Button variant="danger" size={largeText ? "lg" : "md"} onClick={removeAll}>
					Remove All
				</Button>
			</div>
		{:else}
			<Button
				variant="default"
				size={largeText ? "lg" : "md"}
				onClick={openAttachment}
				{icon}
				iconPosition="right"
			>
				{label}
			</Button>
		{/if}

		<br />
		Drag and Drop Files Here
		<br />
		<br />
		<article>
			<ul>
				{#if $processState.imageFilePaths?.length > 0}
					{#each $processState.imageFilePaths as filePath}
						<ImageBubble filename={filePath.name} on:remove={remove(filePath)} />
					{/each}
				{/if}
			</ul>
		</article>
	</Dropzone>
	<br />
</main>

<style lang="postcss">
	main {
		@apply h-full;
	}

	/* Custom dropzone styling - converted from inline styles.js */
	main :global(.custom-dropzone) {
		background-color: var(--color-surface-base);
		color: var(--color-text-tertiary);
		border-color: var(--color-border);
		@apply flex flex-col items-center p-5 border-2 border-dashed 
               rounded-[10px] outline-none transition-all duration-200;
	}

	.button-row {
		@apply flex gap-3 w-full justify-center;
	}
	ul {
		@apply flex flex-col gap-2 w-full justify-center items-center;
	}
	article {
		background-color: var(--color-surface-base);
		@apply w-full min-h-[12rem] max-h-[30rem] overflow-auto rounded-[32px] py-2 px-6;
	}
</style>
