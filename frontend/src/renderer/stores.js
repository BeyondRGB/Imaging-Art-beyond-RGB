import { writable } from 'svelte/store';

export const currentPage = writable(0);

export const appSettings = writable(0);

export const modal = writable(null);