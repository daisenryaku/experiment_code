from template import template_render

def http_response_content_type(filepath):
    if 'html' in filepath.split('.'):
        contentType = 'text/html'
    elif 'png' in filepath.split('.'):
        contentType = 'image/png'
    else:
    	contentType = 'text/plain'
    return contentType

def http_response_from_dict(message_dict):
    """
    Given a dictionary of parameters create a http response
    """
    phrases = {200: 'OK',
               404: 'Not Found',
               400: 'Bad Request',
               }

    status_line = "{0} {1} {2}\n".format(
							    	message_dict['Version'],
							    	message_dict['Status'],
							    	phrases[ message_dict['Status'] ]
    								)

    header_items = [(k,v) for k, v in message_dict.items()
                    if k not in ['Version', 'Status', 'Body']]

    header_lines = "\n".join(["{0}: {1}".format(k, v) for k, v in header_items])

    status_line = status_line.encode('ascii')
    header_lines = header_lines.encode('ascii')

    return b''.join( [status_line, header_lines,
                      b'\n\n',
                      message_dict['Body'],
                      b"\n"] )

def http_response(filepath, arg):
    contentType = http_response_content_type(filepath)
    try:
        with open(filepath, 'rb') as f:
            body = f.read()
            status = 200
    except FileNotFoundError:
        status = 404
        body = b'File not found'

    # template render
    if arg:
        body = template_render(body, arg)

    response_dict = {'Version' : 'HTTP/1.1', 'Status' : status, 'Body' : body, 'Content-Type': contentType}
    return http_response_from_dict(response_dict)
