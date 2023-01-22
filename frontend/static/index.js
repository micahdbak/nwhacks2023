function post(form_id) {
	// Dunno how, but extract content, author and epoch from post form
	var content = extract_content (form_id);
	var author  = extract_author  (form_id);
	var epoch   = get_epoch (now);
	
	var data = {
		cmd: "post " + content + "~" + author + "~" + epoch
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
function string_to_thread (str, path) {
	const parts = text.split ("~");
	
	const thr = new Thread (parseInt(parts[0]), 
							parts[1], parts[2], 
							parseInt(parse[3]),
							path);
	
	return thr;
}

// Retrieves the threads from requested directory
function load_path (path) {
	var data = {
		// FYI: the full path to the directory intended to be listed will be provided
		cmd: 'list ' + path
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

function open_thread (path) {
	threads = load_path(path);
	body = document.getElementById(path + ':body');

	for (thread : threads)
	{
		div = document.createElement('div');
		div.class = 'thread';

		header = document.createElement('div');
		header.class = 'header';

		buttons = document.createElement('div');
		buttons.innerHTML += "<button onclick="open_thread(this.parent().attr('id'))">View</button>\n"
		                     "<button onclick="select(this.parent().attr('id'))">Select</button>";

		header.appendChild(buttons);

		h1 = document.createElement('h1');
		h1.innerHTML = thread.content;
		
		header.appendChild(h1);

		h2 = document.createElement('h2');
		h2.innerHTML = thread.author + ' ' + thread.epoch;

		header.appendChild(h2);

		div.appendChild(header);

		div_body = document.createElement('div');
		div_body.class = 'body';
		div_body.id = thread.path + ':body';

		div.appendChild(div_body);

		div.id = thread.path;

		body.appendChild(div);
	}
}
