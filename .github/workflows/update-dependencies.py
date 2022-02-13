import os
import re
import urllib.request

import atoma

DEPENDENCIES = [
    ("fribidi", "fribidi/fribidi", "v"),
    ("harfbuzz", "harfbuzz/harfbuzz", ""),
    ("openjpeg", "uclouvain/openjpeg", "v"),
]


def update_version(name: str, slug: str, v_in_tag: str) -> str | None:
    url = f"https://github.com/{slug}/tags.atom"
    print(url)
    contents = urllib.request.urlopen(url).read()
    feed = atoma.parse_atom_bytes(contents)
    link = feed.entries[0].links[0].href
    new_tag = link.rsplit("/", 1)[1]
    print(f"{new_tag=}")
    new_version = new_tag.removeprefix("v")
    print(f"{new_version=}")

    filename = "winbuild/build_prepare.py"

    with open(filename) as f:
        content = f.read()
        content_new = re.sub(
            rf"https://github.com/{slug}/archive/{v_in_tag}\d+\.\d+\.\d+.zip",
            f"https://github.com/{slug}/archive/{v_in_tag}{new_version}.zip",
            content,
        )
        content_new = re.sub(
            rf"{name}-\d+\.\d+\.\d+",
            rf"{name}-{new_version}",
            content_new,
        )
    changes_made = content != content_new
    print(f"{changes_made=}")

    if changes_made:
        # Write the file out again
        with open(filename, "w") as f:
            f.write(content_new)
        return f"{name} to {new_version}"

    return None


def main() -> None:
    updates = []
    for name, slug, v_in_tag in DEPENDENCIES:
        update = update_version(name, slug, v_in_tag)
        if update:
            updates.append(update)

    if updates:
        commit_message = "Update " + ", ".join(updates)
        print(commit_message)

        github_env_file = os.getenv("GITHUB_ENV")
        if github_env_file:
            with open(github_env_file, "a") as f:
                f.write(f"COMMIT_MESSAGE={commit_message}")
    else:
        print("No updates")


if __name__ == "__main__":
    main()
