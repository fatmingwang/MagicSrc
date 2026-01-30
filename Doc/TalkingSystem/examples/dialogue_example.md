Dialogue JSON fields (explanation)

- `version`: schema version.
- `entities`: map of `entityId` (string) -> entity data.
- Entity:
  - `alias` (string): human-readable name for debugging. Used in logs and debug tools. Does not affect lookup — the numeric `entityId` is still the key.
  - `nodes`: array of node objects.

## Localization (Multi-language support)

All `text` and `label` fields contain **localization keys** (not actual text).
Actual strings are stored in a separate file: `dialogue_localization.json`.

**Key naming convention:**
- `dlg_{entityId}_{nodeId}_text` — main node text
- `dlg_{entityId}_{nodeId}_opt{index}` — option label
- `dlg_{entityId}_{nodeId}_branch{index}_text` — conditional branch text
- `dlg_{entityId}_{nodeId}_seq{index}_text` — sequence content text
- `dlg_{entityId}_{nodeId}_rnd{index}_text` — random content text
- `dlg_common_*` — shared/reusable strings (goodbye, thanks, ok, etc.)

**Localization file structure:**
```json
{
  "version": "1.0",
  "languages": {
    "en": { "dlg_100_0_text": "Hello, traveler!", ... },
    "zh_TW": { "dlg_100_0_text": "???????", ... },
    "ja": { "dlg_100_0_text": "??????????", ... }
  }
}
```

**Runtime flow:**
1. Load `dialogue_example.json` (structure + keys).
2. Load `dialogue_localization.json` (translations).
3. Set current language (e.g., `en`, `zh_TW`, `ja`).
4. When displaying, lookup: `localization["languages"][currentLang][key]`.
5. Fallback to `en` if key missing in current language.

## Node types

Each node has a `type` field that determines how the node behaves:

### `normal` (default)
Standard dialogue node. Shows `textKey` and `options`.
- `nodeId`, `speaker`, `textKey`, `options`

### `conditional`
Switches content based on flags. Evaluates `branches` in order; first matching branch is used.
- `branches`: array of { `conditions`, `textKey`, `options` }
- Empty `conditions` array means "default/fallback" (put last).
- Example use: quest progress (not started ? in progress ? complete).

### `sequence`
Cycles through content in order each time the player talks. Tracks progress via `sequenceFlag`.
- `sequenceFlag`: persistent flag name to track current index.
- `loop`: if true, wraps back to first content after last. If false, stays on last.
- `contents`: array of { `textKey`, `options` }
- Example use: villager small talk that changes each visit.

### `random`
Randomly picks one content entry each time.
- `contents`: array of { `textKey`, `options` }
- Example use: drunk NPC, fortune teller, ambient chatter.

### `once`
Shows different content on first interaction vs. repeat visits.
- `onceFlag`: persistent flag name to track if seen.
- `firstTime`: { `textKey`, `options`, `setFlags` (optional) } — shown first time.
- `repeat`: { `textKey`, `options` } — shown on subsequent visits.
- Example use: reading a sign, discovering a secret.

### `check`
Performs a game-state check (item, stat, quest) and branches.
- `checkType`: type of check (e.g., `HasItem`, `HasQuest`, `StatGreaterThan`).
- `checkParam`: parameter for the check (e.g., item id, quest id).
- `pass`: { `textKey`, `options` } — shown if check passes.
- `fail`: { `textKey`, `options` } — shown if check fails.
- Example use: guard checking for pass, door requiring key.

## Node fields (common)
- `nodeId` (int): unique ID for the node within this entity.
- `type` (string): one of `normal`, `conditional`, `sequence`, `random`, `once`, `check`.
- `speaker` (string): actor name or portrait key.
- `textKey` (string): localization key for displayed text. Resolved at runtime via localization file.
- `conditions` (optional array of strings): required flags or expressions. Example: ["asked_elder", "!has_item_axe"];
- `options` (array): option objects.

## Option fields
- `labelKey` (string): localization key for option text. Resolved at runtime via localization file.
- `nextNodeId` (int): target node id within current entity. -1 ends the conversation.
- `nextEntityId` (optional string): if specified, jump to a different entity instead of current one. Use with `nextNodeId` to specify which node in the target entity.
- `setFlags` (optional array): flags to set when selected.
- `clearFlags` (optional array): flags to clear when selected.
- `actions` (optional array): action descriptors executed when chosen. Example: "StartQuest:Wood:10" or "GiveItem:Log:10".

## Cross-entity jumping (nextEntityId)
Options can jump to a completely different entity's dialogue using `nextEntityId`.

**How it works:**
- If `nextEntityId` is present, the system switches to that entity.
- `nextNodeId` specifies which node to start in the target entity.
- If `nextEntityId` is absent, `nextNodeId` refers to the current entity (default behavior).

**Use cases:**
| Scenario | Example |
|----------|---------|
| Chain conversations | Elder says "Talk to the guard" ? jumps to guard entity |
| Cutscenes | Throne room event chains King ? Advisor ? back to King ? to Elder for quest |
| Shared dialogues | Multiple NPCs redirect to a common "tutorial" entity |
| Redirect on failure | Guard says "Ask the elder" ? jumps to elder entity |

**Example in JSON:**
```json
{ "label": "Talk to the guard.", "nextEntityId": "600", "nextNodeId": 0 }
```
This option jumps from the current entity to entity `600` (guard), starting at node `0`.

## Alias field — why and how
- The `alias` provides a readable name for each entity (e.g., `npc_elder`, `npc_shopkeeper`).
- Useful for:
  - Log messages: `[Dialogue] Started conversation with npc_elder (ID: 100)`
  - Debug commands: allow starting a conversation by alias in console.
  - Searching logs and setting breakpoints by meaningful names.
- The numeric ID remains the primary key for lookup; alias is purely for human readability.
- Optionally build a reverse map (`alias` -> `entityId`) for debug tools.

## Usage notes
- Use `StartConversation(entityId)` in code when player clicks an NPC or item.
- For `conditional` nodes, evaluate branches top-to-bottom; use first match.
- For `sequence` nodes, increment stored index after showing content.
- For `check` nodes, implement handlers for each `checkType` in code.
- Actions are dispatched to the game (AddItem, StartQuest, OpenShop). Keep action list minimal and implement handlers in code.

## Examples in dialogue_example.json
| Entity ID | Alias              | Node Types Used       | Cross-entity examples |
|-----------|--------------------|-----------------------|-----------------------|
| 100       | npc_elder          | normal, conditional   | Option jumps to 600   |
| 200       | npc_shopkeeper     | normal                | —                     |
| 300       | npc_villager       | sequence, normal      | —                     |
| 400       | npc_drunk          | random                | —                     |
| 500       | item_sign          | once                  | —                     |
| 600       | npc_guard          | check                 | Option jumps to 100   |
| 700       | event_throne_room  | normal (multi-speaker)| Option jumps to 100   |


