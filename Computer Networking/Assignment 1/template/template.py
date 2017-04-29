import re

def template_render(body, arg):
  body = body.decode('utf-8')

  for k in arg:
      restr = r'{%' + k + r'%}'
      pattern = re.compile( restr )
      body = re.sub(pattern, arg[k], body)

  body = body.encode('utf-8')
  return body
