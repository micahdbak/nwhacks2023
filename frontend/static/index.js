function receive() {
	fetch(window.location.href, {
		headers: {
			'Content-Type': 'application/json'
		},
		method: 'POST',
		body: {
			'msg': 'Hello, from the browser!',
		},
	}
	console.log('Receive function run.')
}
