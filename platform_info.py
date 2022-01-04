import platform

print(f'Python {platform.python_version()} {platform.architecture()[0]} on '
    f'{platform.system()} {platform.version()}')
