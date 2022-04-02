from edit_operation import levenshtein

if __name__ == '__main__':
    samples = [
                ('bahal', 'ahal'),
                ('bahal', 'abhal'),
                ('bahal', 'chal'),
                ('bahal', 'cahal'),
                ('bahal', 'cbahal'),
                ('bahal', 'cajal'),
                ('cajal', 'cajalh')
            ]

    for sample in samples:  
        print(sample[0], sample[1])
        operations = levenshtein.edit_operations(sample[0], sample[1], 2, is_damerau=True)
        print(operations)
