import App from '@root/App.svelte';
import App2 from '@root/App2.svelte';
const bodyID = document.getElementsByTagName('body')[0].id;

var app = {};
if (bodyID == "index") {
    app = new App({
        target: document.body,
    });
}
else if (bodyID == "indexChild") {
    app = new App2({
        target: document.body,
    });
}
// const app = new App({
// 	target: document.body,
// 	// props: {
// 	// 	name: 'world'
// 	// }
// });

export default app;