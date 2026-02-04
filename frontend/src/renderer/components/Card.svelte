<!--
  @component
  Generic Card Component
  Provides consistent styling for card-like containers throughout the app
-->
<script lang="ts">
	export let variant: "default" | "elevated" | "dark" | "outlined" = "default";
	export let padding: "none" | "sm" | "md" | "lg" = "md";
	export let rounded: boolean = true;
	export let shadow: "none" | "subtle" | "medium" | "large" = "none";
	export let borderWidth: "none" | "thin" | "medium" | "thick" = "none";
	export let hover: boolean = false;
	export let interactive: boolean = false;
	export let className: string = "";
</script>

<div
	class="card {className}"
	on:click
	class:card-default={variant === "default"}
	class:card-elevated={variant === "elevated"}
	class:card-dark={variant === "dark"}
	class:card-outlined={variant === "outlined"}
	class:padding-none={padding === "none"}
	class:padding-sm={padding === "sm"}
	class:padding-md={padding === "md"}
	class:padding-lg={padding === "lg"}
	class:rounded
	class:shadow-none={shadow === "none"}
	class:shadow-subtle={shadow === "subtle"}
	class:shadow-medium={shadow === "medium"}
	class:shadow-large={shadow === "large"}
	class:border-none={borderWidth === "none"}
	class:border-thin={borderWidth === "thin"}
	class:border-medium={borderWidth === "medium"}
	class:border-thick={borderWidth === "thick"}
	class:card-hover={hover}
	class:card-interactive={interactive}
>
	<slot />
</div>

<style lang="postcss">
	.card {
		@apply relative transition-all duration-200;
	}

	/* Variant styles - NO color literals! */
	.card-default {
		background-color: var(--color-surface);
	}

	.card-elevated {
		background-color: var(--color-surface-elevated);
	}

	.card-dark {
		background-color: var(--color-surface-sunken);
	}

	.card-outlined {
		background-color: transparent;
		border-color: var(--color-border);
	}

	/* Padding options */
	.padding-none {
		@apply p-0;
	}

	.padding-sm {
		@apply p-2;
	}

	.padding-md {
		@apply p-4;
	}

	.padding-lg {
		@apply p-6;
	}

	/* Rounded corners */
	.rounded {
		@apply rounded-xl;
	}

	/* Shadow variants */
	.shadow-none {
		box-shadow: none;
	}

	.shadow-subtle {
		@apply shadow-sm;
	}

	.shadow-medium {
		@apply shadow-md;
	}

	.shadow-large {
		@apply shadow-lg;
	}

	/* Border width options */
	.border-none {
		border-width: 0;
	}

	.border-thin {
		border-width: 1px;
		border-style: solid;
		border-color: var(--color-border);
	}

	.border-medium {
		border-width: 2px;
		border-style: solid;
		border-color: var(--color-border);
	}

	.border-thick {
		border-width: 3px;
		border-style: solid;
		border-color: var(--color-border);
	}

	/* Hover effects */
	.card-hover:hover {
		@apply shadow-md;
		transform: translateY(-1px);
	}

	/* Interactive card (clickable) */
	.card-interactive {
		@apply cursor-pointer;
	}

	.card-interactive:hover {
		background-color: var(--color-interactive-hover);
		@apply shadow-md;
		transform: translateY(-2px);
	}

	.card-interactive:active {
		transform: translateY(0);
		@apply shadow-sm;
	}
</style>
