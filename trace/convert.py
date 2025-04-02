# Copyright (C) 
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
import re
import json
import argparse

# Trace log for parsing
# <...>-40950   [006] .....  8219.402419: tracing_mark_write: E|40950|40950|main|1737333169285502
def parse_trace_line(line):
    # Reg expression
    match = re.match(r'.*\[(\d+)\].*tracing_mark_write: (B|E|X)\|(\d+)\|(\d+)\|(\w+)\|(\d+)', line)
    if match:
        cpu_id, event_type, pid, tid, event_name, timestamp = match.groups()
        return int(cpu_id), event_type, int(pid), int(tid), event_name, int(timestamp)
    return None

def parse_trace_file(filename):
    """
    Parse the trace file and extract events.

    Args:
        trace_file (str): Path to the input trace file.

    Returns:
        list: A list of parsed events.
    """
    events = []
    with open(filename, 'r') as file:
        for line in file:
            parsed = parse_trace_line(line.strip())
            if parsed:
                cpu_id, event_type, pid, tid, event_name, timestamp = parsed
                if event_type == 'B':
                    events.append({
                        'name': event_name,
                        'ph': 'B',
                        'pid': pid,
                        'tid': tid,
                        'ts': timestamp,  # Time stamp
                        'args': {
                            'cpu_id': cpu_id  # Arguments (additional data like cpu_id)
                        }
                    })
                elif event_type == 'E':
                    events.append({
                        'name': event_name,
                        'ph': 'E',
                        'pid': pid,
                        'tid': tid,
                        'ts': timestamp,  # Time stamp
                        'args': {
                            'cpu_id': cpu_id  # Arguments (additional data like cpu_id)
                        }
                    })
                elif event_type == 'X':
                    events.append({
                        'name': event_name,
                        'ph': 'X',
                        'pid': pid,
                        'tid': tid,
                        'ts': timestamp,  # Time stamp
                        'args': {
                            'cpu_id': cpu_id  # Arguments (additional data like cpu_id)
                        }
                    })
                else:
                    # not parsing other types of events
                    continue
    return events

def generate_chrome_trace(events, output_file):
    """
    Generate a Chrome trace JSON file from the given events.

    Args:
        events (list): List of events to be included in the trace.
        output_file (str): Path to the output JSON file.
    """
    with open(output_file, 'w') as file:
        file.write('[\n')
        for i, event in enumerate(events):
            file.write(json.dumps(event, separators=(',', ':')))
            if i < len(events) - 1:
                file.write(',\n')
        file.write('\n]')

if __name__ == '__main__':
    # Set up command line argument parsing
    parser = argparse.ArgumentParser(
        usage="%(prog)s -i <input_file> -o <output_file>",
        description='Convert a trace file to a Chrome Trace Viewer compatible JSON format.'
    )
    parser.add_argument(
        '-i', '--input',
        required=True,
        help='Path to the input trace file (e.g., trace_output.txt).'
    )
    parser.add_argument(
        '-o', '--output',
        required=True,
        help='Path to the output Chrome trace JSON file (e.g., chrome_trace.json).'
    )
    args = parser.parse_args()

    # Parse the trace file and generate the Chrome trace file
    events = parse_trace_file(args.input)
    generate_chrome_trace(events, args.output)
    print(f'Chrome trace file generated: {args.output}')

