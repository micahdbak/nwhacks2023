function post() {
	author = document.getElementById('post_author').value;
	content = document.getElementById('post_content').value;

	data = {
		author: author,
		content: content,
	};

	fetch(window.location.href, {
		headers: {
			'Content-Type': 'application/json'
		},
		method: 'POST',
		body: JSON.stringify(data),
	})
	.then(function(response) {
		if (response.ok) {
			console.log('Sent JSON to server.');

			location.reload();
		} else
			console.log('Couldn\'t send JSON to server.');
	});
}
