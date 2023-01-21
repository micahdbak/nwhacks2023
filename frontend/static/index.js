function receive() {
	var data = {
		msg: "Hello from the browser!"
	};

	console.log(window.location.href)

	fetch(window.location.href, {
		headers : {
			'Content-Type' : 'application/json'
		},
		method : 'POST',
		body : JSON.stringify (data),
	})
	.then(function (response) {
		console.log('Tried fetch.')

		if (response.ok)
			console.log('Sent JSON to server.')
		else
			console.log('Couldn\'t send JSON to server.')
	});

	console.log('After fetch')
}
