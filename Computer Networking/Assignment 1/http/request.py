import itertools

def http_request(request_string):
  assert not isinstance(request_string, bytes)

  try:
    [header, body] = request_string.split('\n\n')
  except ValueError:
    header = request_string
    body = 'nothing'

  header_lines = header.rstrip().split('\n')

  if header_lines[0]:
    method, url, version = header_lines[0].split()
  else:
    print(request_string)

  firstline_dict = {'Method' : method, 'Url' : url, 'Version' : version}

  headers = [l.split(': ') for l in header_lines[1:]]

  if method == 'GET':
     request =  dict(itertools.chain(firstline_dict.items(), ))
  elif method == 'POST' and url == '/':
     request =  dict(itertools.chain(firstline_dict.items(), [('Header', headers[-1][0])]))
  elif method == 'POST':
     request =  dict(itertools.chain(firstline_dict.items()))
  elif method == 'HEAD':
     request = dict(itertools.chain(firstline_dict.items()))

  #if method == 'POST':
      #print(headers)
  return request
