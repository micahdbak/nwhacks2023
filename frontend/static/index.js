function post() {
	var data = {
		type: "post",
		cmd: document.getElementById("1").value
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
			console.log('Sent JSON to server.');
		else
			console.log('Couldn\'t send JSON to server.');
	});
}


// Rerpresents the same thread as in backend
class Thread {
	constructor (_type, _content, _author, _epoch, _path) {
		this.type    = _type;
		this.content = _content;
		this.author  = _author;
		this.epoch   = _epoch;
		this.path = _path;
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
		return this.epoch;
	}
	
	get path () {
		return this.path;
	}
}

// Converts a string of format type~content~author~epoch into thread object
function string_to_thread (var str, var path) {
	const parts = text.split ("~");
	
	const thr = new Thread (parseInt(parts[0]), 
							parts[1], parts[2], 
							parseInt(parse[3]),
							path);
	
	return thr;
}

// Retrieves the threads from requested directory
function load_path (var path, var index) {
	var full_path = "list " + path + "/" + index;
	
	var data = {
		// Command for python socket to run
		cmd: 'list ' + full_path
	};

	console.log(window.location.href)
	
	// Array of Thread objects that represents this directory to-load
	const threads = [];

	fetch (window.location.href, {
		headers : {
			'Content-Type' : 'application/json'
		},
		method: 'POST',
		body: JSON.stringify(data),
	})
	.then (function (response) {
		console.log('Tried fetch.')

		if (response.ok) {
			response.json()
			.then (function (response) {
				// If request failed - return empty array
				if (response.reply == "failure")
					return [];
				// Convert given response into array of threads
				const lines = response.reply.split("\n");
				
				for (var line : lines) {
					threads.push (string_to_thread (line), full_path);
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
