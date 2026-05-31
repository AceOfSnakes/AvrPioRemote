#!/usr/bin/env python3
import os
import re
from datetime import datetime

def get_metadata_from_debian():
    name = "avrpioremote"
    version = "26.06"
    url = "https://github.com"
    
    if os.path.exists('debian/changelog'):
        with open('debian/changelog', 'r') as f:
            first_line = f.readline().strip()
            match = re.search(r'^(\S+)\s+\(([^)]+)\)', first_line)
            if match:
                name = match.group(1)
                version = match.group(2).split('-')[0]

    if os.path.exists('debian/control'):
        with open('debian/control', 'r') as f:
            for line in f:
                if line.strip().startswith('Homepage:'):
                    url = line.replace('Homepage:', '').strip()
                    break
                    
    return name, version, url

def convert_debian_changelog_to_rpm(changelog_path="debian/changelog"):
    if not os.path.exists(changelog_path):
        return "%changelog\n* Sun May 31 2026 Ace Of Snakes <AceOfSnakesMain@gmail.com> - 26.06-1\n- Automated packaging split."

    with open(changelog_path, 'r') as f:
        lines = f.readlines()

    header_re = re.compile(r'^(\S+)\s+\(([^)]+)\)\s+([^;]+);')
    footer_re = re.compile(r'^ \-\- (.+?)\s+<(.+?)>\s+(.+)$')

    entries = []
    current_entry = None
    changes = []

    for line in lines:
        header_match = header_re.match(line)
        footer_match = footer_re.match(line)

        if header_match:
            current_entry = {'version': header_match.group(2).split('-')[0]}
        elif footer_match and current_entry:
            current_entry['maintainer'] = footer_match.group(1)
            current_entry['email'] = footer_match.group(2)
            
            deb_date_str = footer_match.group(3).strip()
            try:
                clean_date = re.sub(r'^[A-Za-z]{3},\s+', '', deb_date_str)
                clean_date = re.sub(r'\s+[\+\-]\d{4}$', '', clean_date)
                dt = datetime.strptime(clean_date, "%d %b %Y %H:%M:%S")
                rpm_date = dt.strftime("%a %b %d %Y")
            except Exception:
                rpm_date = "Sun May 31 2026"

            current_entry['date'] = rpm_date
            current_entry['changes'] = changes
            entries.append(current_entry)
            current_entry = None
            changes = []
        else:
            stripped = line.strip()
            if stripped and not stripped.startswith('--') and stripped != '*':
                item = re.sub(r'^\*\s*', '', stripped)
                changes.append(item)

    rpm_changelog = "%changelog\n"
    for entry in entries:
        rpm_changelog += f"* {entry['date']} {entry['maintainer']} <{entry['email']}> - {entry['version']}-1\n"
        for change in entry['changes']:
            rpm_changelog += f"- {change}\n"
        rpm_changelog += "\n"

    return rpm_changelog.strip()

def generate_spec():
    name, version, url = get_metadata_from_debian()
    pct = "%"

    spec_content = f"""{pct}define _rpmfilename {pct}{pct}{{NAME}}_{pct}{pct}{{VERSION}}_{pct}{pct}{{ARCH}}.rpm
# FIXED: Overrides spec post-install scripts to completely skip doc analysis or compression checks
{pct}define __spec_install_post {pct}{{nil}}
{pct}define __brp_keep_la_files 1

Name:           {name}
Version:        {version}
Release:        1{pct}{{?dist}}
Summary:        AVR PIO Remote application

License:        GPLv3+
URL:            {url}

Source0:        {name}_{version}_x86_64.txz
Source1:        {name}-qt5_{version}_x86_64.txz

BuildRequires:  tar

{pct}description
With this software you are able to control your Pioneer receiver from your PC.
This package contains the binary compiled against the Qt6 framework.

# --- SEPARATE RPM PACKAGE: Qt5 VERSION ---
{pct}package -n {name}-qt5
Summary:        AVR PIO Remote application (Qt5)

{pct}description -n {name}-qt5
With this software you are able to control your Pioneer receiver from your PC.
This package contains the binary compiled against the Qt5 framework.

{pct}prep
rm -rf {pct}{{_builddir}}/{name}-{version}
mkdir -p {pct}{{_builddir}}/{name}-{version}
cd {pct}{{_builddir}}/{name}-{version}

mkdir source-qt6 && tar -xf {pct}{{SOURCE0}} -C source-qt6
mkdir source-qt5 && tar -xf {pct}{{SOURCE1}} -C source-qt5

{pct}build
# Pre-compiled binaries require no compiler tasks

{pct}install
cd {pct}{{_builddir}}/{name}-{version}

rm -rf {pct}{{buildroot}}
mkdir -p {pct}{{buildroot}}

cp -a source-qt6/* {pct}{{buildroot}}/
cp -a source-qt5/* {pct}{{buildroot}}/

# FIXED: Explicitly delete the extracted usr/share/doc folders so RPM never processes them
rm -rf {pct}{{buildroot}}/usr/share/doc

# --- Manifest for Package 1: avrpioremote (Qt6) ---
# FIXED: Removed /usr/share/doc/{name}/ path tracking line
{pct}files
/opt/{name}/
/usr/share/applications/{name}.desktop

# --- Manifest for Package 2: avrpioremote-qt5 (Qt5) ---
# FIXED: Removed /usr/share/doc/{name}-qt5/ path tracking line
{pct}files -n {name}-qt5
/opt/{name}-qt5/
/usr/share/applications/{name}-qt5.desktop
"""

    spec_content += "\n" + convert_debian_changelog_to_rpm() + "\n"

    with open(f"{name}.spec", "w") as spec_file:
        spec_file.write(spec_content)
    print(f"Successfully generated independent dual-RPM spec template ignoring doc paths.")

if __name__ == "__main__":
    generate_spec()
