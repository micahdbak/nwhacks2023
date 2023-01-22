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

// Rerpresents the same thread as in backend
class Thread {
	constructor (_type, _content, _author, _epoch) {
		this.type    = _type;
		this.content = _content;
		this.author  = _author;
		this.epoch   = _epoch;
	}
	
	get type () {
		return this.type;
	}
	
	get content () {
		return this.content;
	}
	
	get author () {
		return this.author;
	}
	
	get epoch () {
		retrun this.epoch;
	}
}


function string_to_thread (var str) {
	const parts = text.split ("~");
	
	const thr = new Thread (parseInt(parts[0]), parts[1], parts[2], parseInt(parse[3]));
	
	return thr;
}


function load_path (var path, var index) {
	var data = {
		type: "list";
		// Command for python socket to run
		cmd: "list " + path + "/" + index;
	};

	console.log(window.location.href)

	const threads = [];

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
				// Convert given response into array of threads
				const lines = response.threads.split("\n");
				
				for (var line : lines) {
					threads.push (string_to_thread (line));
				}
			});
		}
		else {
			console.log('Couldn\'t send JSON to server.');
		}
	})
	.catch (function (error) {
		console.log(error);
	});
	
	return threads;
}