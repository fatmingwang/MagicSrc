# Prototype V1 — Playflow & Interaction (步兵/工兵/騎兵)

目標：建立能示範核心玩法的最小可行原型（MVP）。僅實作三種單位：步兵（Infantry）、工兵（Worker）、騎兵（Cavalry），並支援 5 個主面板的玩家互動（Gather/Fight/Research/Forge/Fortify）。

1) 概要
- 玩家介面由 5 個主要按鈕啟動浮動面板（Modal/Tab/Split）。
- 原型重點：招募/派遣/升級/裝備/戰鬥結算與簡易守城。
- 遊戲循環：資源採集 → 生產/鍛造/研究 → 編隊出征或駐守 → 戰鬥自動結算 → 經驗與升級。

2) 資料模型（最小）
- PlayerState: { resources: {meat,money,metal,wood}, population, unitList[] }
- Unit: { id, kind: "worker|infantry|cavalry", level, exp, stats:{HP,attack,defense,speed,range}, equipment[] }
- QueueItem: { type: "recruit|produce|research", targetId, finishAt }
- EnemyWave: { id, units: [{kind,level,count}], spawnTime }

3) UI 與互動流程（每面板皆有示範 API 事件）

Gather 面板（工兵）
- 顯示：工人數、空閒工人、採集任務、資源數量。
- 按鈕與行為：
  - `Recruit Worker`：消耗 {meat:10,money:5}，加入招募隊列（QueueItem），完成後新增 Unit(kind=worker)。
  - `Assign Gather`：從空閒工人列表選擇 N 名並指定資源類型（wood/metal/meat/money）。系統開始每秒產出資源並同時按 expRate 給工人 exp。
  - `Auto-Collect Toggle`：開啟後空閒工人自動派工。
- 升級（Level Up）：當 worker.exp >= expToNext 時，玩家可在工人欄位點 `Level Up` 消耗少量資源以升級（直接升級或立即生效）。
- 事件示例：`onAssignGather(workerIds, resourceType)`、`onRecruitWorker()`。

Forge 面板（鍛造）
- 顯示：原料庫存、生產佇列、可製造物品（Basic Tool / Iron Sword）。
- 按鈕：
  - `Produce Tool`：消耗材料並在生產完成後自動加入庫存或直接裝備給指定單位。
  - `Auto-Equip`（簡化）：若選中，完成生產會自動分配給最適合的單位。
- 事件示例：`onProduce(itemId, qty, autoEquipToUnitId?)`。

Research 面板（研究）
- 顯示：可研究項目列表（Worker Exp +10%，Unit Attack +5% 等）與研究時間。
- 互動：選擇研究項目後加入研究隊列，研究完畢後修改全域數值或單位屬性。
- 事件示例：`onStartResearch(researchId)`。

Fortify 面板（防禦）
- 顯示：現有防禦建築、駐守人力、彈藥/維修包。
- 互動：
  - `Build ArrowTower`：消耗資源並占用格位，完成後可指派駐守。駐守會給駐守單位少量 exp。
  - `Assign Crew`：把指定單位指派到防禦設施，提升該設施效率。
- 事件示例：`onBuild(defenseType)`、`onAssignCrew(unitId, defenseId)`。

Fight 面板（戰鬥）
- 顯示：當前可用部隊、隊列、可出征的路徑（單軸）與組合技按鈕。
- 互動：
  - `Recruit Infantry/Cavalry`：消耗資源排入招募隊列。
  - `Set Attack Path`：選擇單軸或多人模式的路徑（原型僅單軸）。
  - `Deploy Force`：從可用單位選擇出征陣列，開始模擬戰鬥。
  - `Trigger Combo`：若滿足需求（例如 3 步兵 + 2 騎兵），玩家可在出征前或戰鬥中觸發一次組合技（消耗冷卻/資源）。
- 事件示例：`onDeploy(units[], pathId)`、`onTriggerCombo(comboId)`。

4) 戰鬥結算（簡化模型）
- 使用回合式模擬（每步代表時間片）：
  - 每回合計算攻擊：每個單位按其攻擊速率對範圍內目標造成傷害。
  - 傷害計算：damage = attack * (1 - target.defense * 0.01)
  - 單位死亡：HP <= 0 則移除。
  - 勝利條件：敵方全部死亡 → 玩家勝利；玩家全部死亡 → 防守/攻擊失敗。
- 經驗機制：每次擊殺給予擊殺者 exp（例：10 EXP），參與傷害按比例分配經驗。
- 駐守加成：防禦建築對駐守單位提供溫和加成（+10% defense）。

5) 升級流程（UI 互動範例）
- 玩家點開任一單位卡（在 Fight / Gather / Fortify 中都可看到）→ 顯示詳細面板（level, exp, stats, nextLevelPreview）→ 若 exp>=expToNext 顯示 `Level Up` 按鈕。
- 點 `Level Up`：消耗小額資源（可設為 0 作為原型）→ 升級後立即增加 stats 並重置 exp（或減去 expToNext）。

6) 原型場景與演示步驟（演示用劇本）
- 初始資源： meat:200, money:500, metal:200, wood:200
- 預設單位： 2 workers (idle), 0 infantry, 0 cavalry
- 演示流程：
  1. 玩家到 Gather 面板招募 1 worker → 完成後 assign to wood → 開始產木並逐步取得 exp。
  2. 玩家到 Fight 面板招募 1 infantry（排隊 15s）並開始生產；同時在 Forge 生產 Basic Sword 為 infantry 裝備。
  3. 玩家部署 infantry 到出征路徑，觸發單次戰鬥模擬，展示攻擊動畫（可為文字或簡單 UI 數字變化）→ 結算勝利或失敗並顯示獲得經驗/資源獎勵。
  4. 玩家將步兵駐守到 Fortify 的 Arrow Tower，展示駐守加成與駐守經驗獲取。
  5. 玩家在 Research 面板啟動 `Worker Exp +10%`，示範研究完成後工人採集及升級加速。

7) 簡單事件 API（供開發實作）
- initPrototype(): 建立初始 PlayerState 與場景。
- tick(deltaTime): 更新佇列、資源產出、戰鬥模擬、研究進度。
- ui.on("recruit", {kind})
- ui.on("assignGather", {unitIds, resourceType})
- ui.on("deploy", {unitIds, pathId})
- ui.on("produce", {itemId})
- ui.on("startResearch", {researchId})
- ui.on("buildDefense", {type})
- ui.on("assignCrew", {unitId, defenseId})

8) 建議輸出格式
- 使用 JSON 作為遊戲狀態序列化（方便前端或測試自動化）。
- 範例回傳（deploy 結算）:
  {
    "result":"win",
    "lostUnits":[/*ids*/],
    "gainedExp":[{"unitId":1,"exp":20}],
    "rewards":{"meat":10,"money":50}
  }
