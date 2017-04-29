import os

def http_urls(request):
	# deal post
	if request['Method'] == 'POST' and request['Url'] == '/':
		arg = {'value':request['Header'][6:]}
		filepath = 'receive.html'
		return filepath, arg

	elif request['Method'] == 'POST':
		filepath = 'upload.html'
		return filepath, None

	filepath = request['Url'][1:]
	arg = None

	# get post
	if '?value=' == filepath[:7]:
		arg = {'value':filepath[7:]}
		filepath = 'receive.html'
		return filepath, arg

	filepath = os.path.abspath( filepath )

	if os.path.isdir(filepath):
		filepath = 'home.html'

	elif not os.path.exists(filepath):
		filepath = '404.html'

	return filepath, arg
