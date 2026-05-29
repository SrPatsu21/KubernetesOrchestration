import { useEffect, useState } from "react";

function App() {
  const [items, setItems] = useState([]);
  const [name, setName] = useState("");

  const API_URL = import.meta.env.VITE_API_URL;

  async function fetchItems() {
    const res = await fetch(`${API_URL}/items`);
    const data = await res.json();
    setItems(data);
  }

  async function addItem() {
    if (!name) return;

    await fetch(`${API_URL}/items`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ name }),
    });

    setName("");
    fetchItems();
  }

  async function deleteItem(id) {
    await fetch(`${API_URL}/items/${id}`, {
      method: "DELETE",
    });

    fetchItems();
  }

  useEffect(() => {
    fetchItems();
  }, []);

  return (
    <div style={{ padding: 20 }}>
      <h1>AngelCorp Items</h1>

      <input
        type="text"
        placeholder="Novo item"
        value={name}
        onChange={(e) => setName(e.target.value)}
      />

      <button onClick={addItem}>
        Adicionar
      </button>

      <ul>
        {items.map((item) => (
          <li key={item.id}>
            {item.name}

            <button onClick={() => deleteItem(item.id)}>
              Remover
            </button>
          </li>
        ))}
      </ul>
    </div>
  );
}

export default App;