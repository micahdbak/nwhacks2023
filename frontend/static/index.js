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
			console.log('Sent JSON to server.');
		else
			console.log('Couldn\'t send JSON to server.');
	});

	console.log('After fetch')
}


function load_path (var path, var index) {
	var data = {
		type: "list";
		cmd: "list " + path + "/" + index;
	};

	console.log(window.location.href)

	fetch (window.location.href, {
		headers : {
			'Content-Type' : 'application/json'
		},
		method : 'POST',
		body : JSON.stringify (data),
	})
	.then (function (response) {
		console.log('Tried fetch.')

		if (response.ok) {
			response.json()
			.then (function (response) {
				
			});
		}
		else {
			console.log('Couldn\'t send JSON to server.');
		}
	})
	.catch (function (error) {
		console.log(error);
	});
}