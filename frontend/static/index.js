function post() {
	var data = {
		msg: document.getElementById("1").value
	};

	fetch(window.location.href, {
		headers: {
			'Content-Type': 'application/json'
		},
		method: 'POST',
		body: JSON.stringify(data),
	})
	.then(function(response) {
		if (response.ok)
			console.log('Sent JSON to server.')
		else
			console.log('Couldn\'t send JSON to server.')
	});
}
